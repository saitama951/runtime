// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System.Buffers;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Net;
using System.Runtime.CompilerServices;
using System.Runtime.Serialization;
using System.Text;
using System.Threading;

namespace System
{
    [Serializable]
    [TypeForwardedFrom("System, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089")]
    public partial class Uri : ISpanFormattable, IEquatable<Uri>, ISerializable
    {
        public static readonly string UriSchemeFile = UriParser.FileUri.SchemeName;
        public static readonly string UriSchemeFtp = UriParser.FtpUri.SchemeName;
        public static readonly string UriSchemeSftp = "sftp";
        public static readonly string UriSchemeFtps = "ftps";
        public static readonly string UriSchemeGopher = UriParser.GopherUri.SchemeName;
        public static readonly string UriSchemeHttp = UriParser.HttpUri.SchemeName;
        public static readonly string UriSchemeHttps = UriParser.HttpsUri.SchemeName;
        public static readonly string UriSchemeWs = UriParser.WsUri.SchemeName;
        public static readonly string UriSchemeWss = UriParser.WssUri.SchemeName;
        public static readonly string UriSchemeMailto = UriParser.MailToUri.SchemeName;
        public static readonly string UriSchemeNews = UriParser.NewsUri.SchemeName;
        public static readonly string UriSchemeNntp = UriParser.NntpUri.SchemeName;
        public static readonly string UriSchemeSsh = "ssh";
        public static readonly string UriSchemeTelnet = UriParser.TelnetUri.SchemeName;
        public static readonly string UriSchemeNetTcp = UriParser.NetTcpUri.SchemeName;
        public static readonly string UriSchemeNetPipe = UriParser.NetPipeUri.SchemeName;
        public static readonly string SchemeDelimiter = "://";

        private const int SchemeLengthLimit = 1024;

        internal const int StackallocThreshold = 512;

        // untouched user string unless string has unicode chars and iriparsing is enabled
        // or idn is on and we have unicode host or idn host
        // In that case, this string is normalized, stripped of bidi chars, and validated
        // with char limits
        private string _string;

        // untouched user string if string has unicode with iri on or unicode/idn host with idn on
        private string _originalUnicodeString = null!; // initialized in ctor via helper

        internal UriParser _syntax = null!;   // Initialized in ctor via helper. This is a whole Uri syntax, not only the scheme name

        internal Flags _flags;
        private UriInfo _info = null!;

        [Flags]
        internal enum Flags : ulong
        {
            Zero = 0,

            SchemeNotCanonical = 1UL << 0,
            UserNotCanonical = 1UL << 1,
            HostNotCanonical = 1UL << 2,
            PortNotCanonical = 1UL << 3,
            PathNotCanonical = 1UL << 4,
            QueryNotCanonical = 1UL << 5,
            FragmentNotCanonical = 1UL << 6,
            CannotDisplayCanonical = SchemeNotCanonical | UserNotCanonical | HostNotCanonical | PortNotCanonical | PathNotCanonical | QueryNotCanonical | FragmentNotCanonical,

            E_UserNotCanonical = 1UL << 7,
            E_HostNotCanonical = 1UL << 8,
            E_PortNotCanonical = 1UL << 9,
            E_PathNotCanonical = 1UL << 10,
            E_QueryNotCanonical = 1UL << 11,
            E_FragmentNotCanonical = 1UL << 12,
            E_CannotDisplayCanonical = E_UserNotCanonical | E_HostNotCanonical | E_PortNotCanonical | E_PathNotCanonical | E_QueryNotCanonical | E_FragmentNotCanonical,

            ShouldBeCompressed = 1UL << 13,
            FirstSlashAbsent = 1UL << 14,
            BackslashInPath = 1UL << 15,

            IndexMask = 0xFFFFFFFF, // 32 bits

            HostTypeMask = 7 * (1UL << 32),
            HostNotParsed = 0,
            IPv6HostType = 1 * (1UL << 32),
            IPv4HostType = 2 * (1UL << 32),
            DnsHostType = 3 * (1UL << 32),
            UncHostType = 4 * (1UL << 32),
            BasicHostType = 5 * (1UL << 32),
            UnusedHostType = 6 * (1UL << 32),
            UnknownHostType = 7 * (1UL << 32),

            UserEscaped = 1UL << 35,
            AuthorityFound = 1UL << 36,
            HasUserInfo = 1UL << 37,
            LoopbackHost = 1UL << 38,
            NotDefaultPort = 1UL << 39,

            UserDrivenParsing = 1UL << 40,
            CanonicalDnsHost = 1UL << 41,
            ErrorOrParsingRecursion = 1UL << 42,    // Used to signal a default parser error and also to confirm Port
                                                    // and Host values in case of a custom user Parser
            DosPath = 1UL << 43,
            UncPath = 1UL << 44,
            ImplicitFile = 1UL << 45,
            MinimalUriInfoSet = 1UL << 46,
            AllUriInfoSet = 1UL << 47,
            IdnHost = 1UL << 48,
            HasUnicode = 1UL << 49,

            // Is this component Iri canonical
            UserIriCanonical = 1UL << 50,
            PathIriCanonical = 1UL << 51,
            QueryIriCanonical = 1UL << 52,
            FragmentIriCanonical = 1UL << 53,
            IriCanonical = UserIriCanonical | PathIriCanonical | QueryIriCanonical | FragmentIriCanonical,
            UnixPath = 1UL << 54,

            /// <summary>
            /// Disables any validation/normalization past the authority. Fragments will always be empty. GetComponents will throw for Path/Query.
            /// </summary>
            DisablePathAndQueryCanonicalization = 1UL << 55,

            /// <summary>
            /// Used to ensure that InitializeAndValidate is only called once per Uri instance and only from an override of InitializeAndValidate
            /// </summary>
            CustomParser_ParseMinimalAlreadyCalled = 1UL << 56,

            /// <summary>
            /// Used for asserting that certain methods are only called from the constructor to validate thread-safety assumptions
            /// </summary>
            Debug_LeftConstructor = 1UL << 57
        }

        [Conditional("DEBUG")]
        private void DebugSetLeftCtor()
        {
            _flags |= Flags.Debug_LeftConstructor;

            AssertInvariants();
        }

        [Conditional("DEBUG")]
        private void AssertInvariants()
        {
            Debug.Assert(InFact(Flags.MinimalUriInfoSet) == (_info is not null));

            if (_info is UriInfo info)
            {
                Debug.Assert(IsAbsoluteUri);

                Offset offset = info.Offset;

                Debug.Assert(offset.Scheme >= 0);
                Debug.Assert(offset.User >= 0 && offset.User >= offset.Scheme);
                Debug.Assert(offset.Host >= 0 && offset.Host >= offset.User);
                Debug.Assert(offset.Path >= 0);
                Debug.Assert(offset.End >= 0 && offset.End >= offset.Path);

                if (InFact(Flags.AllUriInfoSet))
                {
                    Debug.Assert(offset.Path >= offset.Host);
                    Debug.Assert(offset.Query >= 0 && offset.Query >= offset.Path);
                    Debug.Assert(offset.Fragment >= 0 && offset.Fragment >= offset.Query);
                    Debug.Assert(offset.End >= offset.Fragment && offset.End <= _string.Length);
                }
                else
                {
                    // If we have non-ASCII, we're about to continue with ParseRemaining.
                    // Between CreateUriInfo and the rest of ParseRemaining, the Path offset is pointing into the original string,
                    // while the Host offset is poining into the new _string. As such the Host offset may temporarily be > Path.
                    if (!InFact(Flags.HasUnicode))
                    {
                        Debug.Assert(offset.Path >= offset.Host);
                    }

                    Debug.Assert(offset.Query == 0);
                    Debug.Assert(offset.Fragment == 0);
                }
            }
            else
            {
                if (IsAbsoluteUri && IriParsing)
                {
                    Debug.Assert(Ascii.IsValid(_string));
                }
            }
        }

        [Conditional("DEBUG")]
        internal void DebugAssertInCtor()
        {
            Debug.Assert((_flags & Flags.Debug_LeftConstructor) == 0);
        }

        private sealed class UriInfo
        {
            public Offset Offset;
            public string? String;
            public string? Host;
            public string? IdnHost;
            public string? PathAndQuery;

            public MoreInfo? _moreInfo;
            public MoreInfo MoreInfo
            {
                get
                {
                    if (_moreInfo is null)
                    {
                        Interlocked.CompareExchange(ref _moreInfo, new MoreInfo(), null);
                    }
                    return _moreInfo;
                }
            }
        };

        private struct Offset
        {
            public int Scheme;
            public int User;
            public int Host;
            public ushort PortValue;
            public int Path;
            public int Query;
            public int Fragment;
            public int End;
        };

        private sealed class MoreInfo
        {
            public string? Path;
            public string? Query;
            public string? Fragment;
            public string? AbsoluteUri;
            public string? RemoteUrl;

            /// <summary>
            /// Only IP v6 may need this
            /// </summary>
            public string? ScopeId;
        };

        private void InterlockedSetFlags(Flags flags)
        {
            Debug.Assert(_syntax != null);

            if (_syntax.IsSimple)
            {
                // For built-in (simple) parsers, it is safe to do an Interlocked update here
                Debug.Assert(sizeof(Flags) == sizeof(ulong));
                Interlocked.Or(ref Unsafe.As<Flags, ulong>(ref _flags), (ulong)flags);
            }
            else
            {
                // Custom parsers still use a lock in CreateHostString and perform non-atomic flags updates
                // We have to take the lock to ensure flags access synchronization if CreateHostString and ParseRemaining are called concurrently
                lock (_info)
                {
                    _flags |= flags;
                }
            }
        }

        private bool IsImplicitFile
        {
            get { return (_flags & Flags.ImplicitFile) != 0; }
        }

        private bool IsUncOrDosPath
        {
            get { return (_flags & (Flags.UncPath | Flags.DosPath)) != 0; }
        }

        private bool IsDosPath
        {
            get { return (_flags & Flags.DosPath) != 0; }
        }

        private bool IsUncPath
        {
            get { return (_flags & Flags.UncPath) != 0; }
        }

        private bool IsUnixPath
        {
            get { return (_flags & Flags.UnixPath) != 0; }
        }

        private Flags HostType
        {
            get { return _flags & Flags.HostTypeMask; }
        }

        private UriParser Syntax
        {
            get
            {
                return _syntax;
            }
        }

        private bool IsNotAbsoluteUri
        {
            get { return _syntax is null; }
        }

        //
        // Checks if Iri parsing is allowed by the syntax & by config
        //
        private bool IriParsing => IriParsingStatic(_syntax);

        internal static bool IriParsingStatic(UriParser? syntax)
        {
            return syntax is null || syntax.InFact(UriSyntaxFlags.AllowIriParsing);
        }

        internal bool DisablePathAndQueryCanonicalization => (_flags & Flags.DisablePathAndQueryCanonicalization) != 0;

        internal bool UserDrivenParsing
        {
            get
            {
                return (_flags & Flags.UserDrivenParsing) != 0;
            }
        }

        private int SecuredPathIndex
        {
            get
            {
                // This is one more trouble with a Dos Path.
                // This property gets "safe" first path slash that is not the first if path = c:\
                if (IsDosPath)
                {
                    char ch = _string[_info.Offset.Path];
                    return (ch == '/' || ch == '\\') ? 3 : 2;
                }
                return 0;
            }
        }

        private bool NotAny(Flags flags)
        {
            return (_flags & flags) == 0;
        }

        private bool InFact(Flags flags)
        {
            return (_flags & flags) != 0;
        }

        private static bool StaticNotAny(Flags allFlags, Flags checkFlags)
        {
            return (allFlags & checkFlags) == 0;
        }

        private static bool StaticInFact(Flags allFlags, Flags checkFlags)
        {
            return (allFlags & checkFlags) != 0;
        }

        [MemberNotNull(nameof(_info))]
        private UriInfo EnsureUriInfo()
        {
            Flags cF = _flags;
            if ((cF & Flags.MinimalUriInfoSet) == 0)
            {
                CreateUriInfo(cF);
                AssertInvariants();
            }
            Debug.Assert(_info != null && (_flags & Flags.MinimalUriInfoSet) != 0);
            return _info;
        }

        private void EnsureParseRemaining()
        {
            if ((_flags & Flags.AllUriInfoSet) == 0)
            {
                ParseRemaining();
                AssertInvariants();
            }
        }

        [MemberNotNull(nameof(_info))]
        private void EnsureHostString(bool allowDnsOptimization)
        {
            UriInfo info = EnsureUriInfo();

            if (info.Host is null)
            {
                if (allowDnsOptimization && InFact(Flags.CanonicalDnsHost))
                {
                    /* Optimization for a canonical DNS name
                    *  ATTN: the host string won't be created,
                    *  Hence ALL _info.Host callers first call EnsureHostString(false)
                    *  For example IsLoopBack property is one of such callers.
                    */
                    return;
                }
                CreateHostString();
            }
        }

        //
        // Uri(string)
        //
        //  We expect to create a Uri from a display name - e.g. that was typed by
        //  a user, or that was copied & pasted from a document. That is, we do not
        //  expect already encoded URI to be supplied.
        //
        public Uri([StringSyntax(StringSyntaxAttribute.Uri)] string uriString)
        {
            ArgumentNullException.ThrowIfNull(uriString);

            CreateThis(uriString, false, UriKind.Absolute);
            DebugSetLeftCtor();
        }

        //
        // Uri(string, bool)
        //
        //  Uri constructor. Assumes that input string is canonically escaped
        //
        [Obsolete("This constructor has been deprecated; the dontEscape parameter is always false. Use Uri(string) instead.")]
        public Uri([StringSyntax(StringSyntaxAttribute.Uri)] string uriString, bool dontEscape)
        {
            ArgumentNullException.ThrowIfNull(uriString);

            CreateThis(uriString, dontEscape, UriKind.Absolute);
            DebugSetLeftCtor();
        }

        //
        // Uri(Uri, string, bool)
        //
        //  Uri combinatorial constructor. Do not perform character escaping if
        //  DontEscape is true
        //
        [Obsolete("This constructor has been deprecated; the dontEscape parameter is always false. Use Uri(Uri, string) instead.")]
        public Uri(Uri baseUri, string? relativeUri, bool dontEscape)
        {
            ArgumentNullException.ThrowIfNull(baseUri);

            if (!baseUri.IsAbsoluteUri)
                throw new ArgumentOutOfRangeException(nameof(baseUri));

            CreateUri(baseUri, relativeUri, dontEscape);
            DebugSetLeftCtor();
        }

        //
        // Uri(string, UriKind);
        //
        public Uri([StringSyntax(StringSyntaxAttribute.Uri, nameof(uriKind))] string uriString, UriKind uriKind)
        {
            ArgumentNullException.ThrowIfNull(uriString);

            CreateThis(uriString, false, uriKind);
            DebugSetLeftCtor();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Uri"/> class with the specified URI and additional <see cref="UriCreationOptions"/>.
        /// </summary>
        /// <param name="uriString">A string that identifies the resource to be represented by the <see cref="Uri"/> instance.</param>
        /// <param name="creationOptions">Options that control how the <seealso cref="Uri"/> is created and behaves.</param>
        public Uri([StringSyntax(StringSyntaxAttribute.Uri)] string uriString, in UriCreationOptions creationOptions)
        {
            ArgumentNullException.ThrowIfNull(uriString);

            CreateThis(uriString, false, UriKind.Absolute, in creationOptions);
            DebugSetLeftCtor();
        }

        //
        // Uri(Uri, string)
        //
        //  Construct a new Uri from a base and relative URI. The relative URI may
        //  also be an absolute URI, in which case the resultant URI is constructed
        //  entirely from it
        //
        public Uri(Uri baseUri, string? relativeUri)
        {
            ArgumentNullException.ThrowIfNull(baseUri);

            if (!baseUri.IsAbsoluteUri)
                throw new ArgumentOutOfRangeException(nameof(baseUri));

            CreateUri(baseUri, relativeUri, false);
            DebugSetLeftCtor();
        }

        //
        // Uri(SerializationInfo, StreamingContext)
        //
        // ISerializable constructor
        //
        [Obsolete(Obsoletions.LegacyFormatterImplMessage, DiagnosticId = Obsoletions.LegacyFormatterImplDiagId, UrlFormat = Obsoletions.SharedUrlFormat)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        protected Uri(SerializationInfo serializationInfo, StreamingContext streamingContext)
        {
            string? uriString = serializationInfo.GetString("AbsoluteUri"); // Do not rename (binary serialization)

            if (uriString!.Length != 0)
            {
                CreateThis(uriString, false, UriKind.Absolute);
                DebugSetLeftCtor();
                return;
            }

            uriString = serializationInfo.GetString("RelativeUri");  // Do not rename (binary serialization)
            if (uriString is null)
                throw new ArgumentException(SR.Format(SR.InvalidNullArgument, "RelativeUri"), nameof(serializationInfo));

            CreateThis(uriString, false, UriKind.Relative);
            DebugSetLeftCtor();
        }

        //
        // ISerializable method
        //
        /// <internalonly/>
        void ISerializable.GetObjectData(SerializationInfo serializationInfo, StreamingContext streamingContext)
        {
            GetObjectData(serializationInfo, streamingContext);
        }

        //
        // FxCop: provide some way for derived classes to access GetObjectData even if the derived class
        // explicitly re-inherits ISerializable.
        //
        protected void GetObjectData(SerializationInfo serializationInfo, StreamingContext streamingContext)
        {

            if (IsAbsoluteUri)
                serializationInfo.AddValue("AbsoluteUri", GetParts(UriComponents.SerializationInfoString, UriFormat.UriEscaped)); // Do not rename (binary serialization)
            else
            {
                serializationInfo.AddValue("AbsoluteUri", string.Empty); // Do not rename (binary serialization)
                serializationInfo.AddValue("RelativeUri", GetParts(UriComponents.SerializationInfoString, UriFormat.UriEscaped)); // Do not rename (binary serialization)
            }
        }

        [MemberNotNull(nameof(_string))]
        private void CreateUri(Uri baseUri, string? relativeUri, bool dontEscape)
        {
            DebugAssertInCtor();

            // Parse relativeUri and populate Uri internal data.
            CreateThis(relativeUri, dontEscape, UriKind.RelativeOrAbsolute);

            if (baseUri.Syntax!.IsSimple)
            {
                // Resolve Uris if possible OR get merged Uri String to re-parse below
                Uri? uriResult = ResolveHelper(baseUri, this, ref relativeUri, ref dontEscape);

                // If resolved into a Uri then we build from that Uri
                if (uriResult != null)
                {
                    if (!ReferenceEquals(this, uriResult))
                        CreateThisFromUri(uriResult);

                    return;
                }
            }
            else
            {
                dontEscape = false;
                relativeUri = baseUri.Syntax.InternalResolve(baseUri, this, out UriFormatException? e);
                if (e != null)
                    throw e;
            }

            _flags = Flags.Zero;
            _info = null!;
            _syntax = null!;
            _originalUnicodeString = null!;
            // If not resolved, we reparse modified Uri string and populate Uri internal data.
            CreateThis(relativeUri, dontEscape, UriKind.Absolute);
        }

        //
        // Uri(Uri , Uri )
        // Note: a static Create() method should be used by users, not this .ctor
        //
        public Uri(Uri baseUri, Uri relativeUri)
        {
            ArgumentNullException.ThrowIfNull(baseUri);

            if (!baseUri.IsAbsoluteUri)
                throw new ArgumentOutOfRangeException(nameof(baseUri));

            CreateThisFromUri(relativeUri);

            string? newUriString = null;
            bool dontEscape;

            if (baseUri.Syntax!.IsSimple)
            {
                dontEscape = InFact(Flags.UserEscaped);
                Uri? resolvedRelativeUri = ResolveHelper(baseUri, this, ref newUriString, ref dontEscape);

                if (resolvedRelativeUri != null)
                {
                    if (!ReferenceEquals(this, resolvedRelativeUri))
                        CreateThisFromUri(resolvedRelativeUri);

                    DebugSetLeftCtor();
                    return;
                }
            }
            else
            {
                dontEscape = false;
                newUriString = baseUri.Syntax.InternalResolve(baseUri, this, out UriFormatException? e);
                if (e != null)
                    throw e;
            }

            _flags = Flags.Zero;
            _info = null!;
            _syntax = null!;
            _originalUnicodeString = null!;
            CreateThis(newUriString, dontEscape, UriKind.Absolute);
            DebugSetLeftCtor();
        }

        //
        // This method is shared by base+relative Uris constructors and is only called from them.
        // The assumptions:
        //  - baseUri is a valid absolute Uri
        //  - relative part is not null and not empty
        private static void GetCombinedString(Uri baseUri, string relativeStr,
            bool dontEscape, ref string? result)
        {
            // NB: This is not RFC2396 compliant although it is inline with w3c.org recommendations
            // This parser will allow the relativeStr to be an absolute Uri with the different scheme
            // In fact this is strict violation of RFC2396
            //
            for (int i = 0; i < relativeStr.Length; ++i)
            {
                if (relativeStr[i] == '/' || relativeStr[i] == '\\' || relativeStr[i] == '?' || relativeStr[i] == '#')
                {
                    break;
                }
                else if (relativeStr[i] == ':')
                {
                    if (i < 2)
                    {
                        // Note we don't support one-letter Uri schemes.
                        // Hence anything like x:sdsd is a relative path and be added to the baseUri Path
                        break;
                    }

                    ParsingError error = ParsingError.None;
                    UriParser? syntax = CheckSchemeSyntax(relativeStr.AsSpan(0, i), ref error);

                    if (error == ParsingError.None)
                    {
                        if (baseUri.Syntax == syntax)
                        {
                            //Remove the scheme for backward Uri parsers compatibility
                            if (i + 1 < relativeStr.Length)
                            {
                                relativeStr = relativeStr.Substring(i + 1);
                            }
                            else
                            {
                                relativeStr = string.Empty;
                            }
                        }
                        else
                        {
                            // This is the place where we switch the scheme.
                            // Return relative part as the result Uri.
                            result = relativeStr;
                            return;
                        }
                    }
                    break;
                }
            }

            if (relativeStr.Length == 0)
            {
                result = baseUri.OriginalString;
            }
            else
            {
                result = CombineUri(baseUri, relativeStr, dontEscape ? UriFormat.UriEscaped : UriFormat.SafeUnescaped);
            }
        }

        private static UriFormatException? GetException(ParsingError err)
        {
            switch (err)
            {
                case ParsingError.None:
                    return null;
                // Could be OK for Relative Uri
                case ParsingError.BadFormat:
                    return new UriFormatException(SR.net_uri_BadFormat);
                case ParsingError.BadScheme:
                    return new UriFormatException(SR.net_uri_BadScheme);
                case ParsingError.BadAuthority:
                    return new UriFormatException(SR.net_uri_BadAuthority);
                case ParsingError.EmptyUriString:
                    return new UriFormatException(SR.net_uri_EmptyUri);
                // Fatal
                case ParsingError.SchemeLimit:
                    return new UriFormatException(SR.net_uri_SchemeLimit);
                case ParsingError.MustRootedPath:
                    return new UriFormatException(SR.net_uri_MustRootedPath);
                // Derived class controllable
                case ParsingError.BadHostName:
                    return new UriFormatException(SR.net_uri_BadHostName);
                case ParsingError.NonEmptyHost: //unix-only
                    return new UriFormatException(SR.net_uri_BadFormat);
                case ParsingError.BadPort:
                    return new UriFormatException(SR.net_uri_BadPort);
                case ParsingError.BadAuthorityTerminator:
                    return new UriFormatException(SR.net_uri_BadAuthorityTerminator);
                case ParsingError.CannotCreateRelative:
                    return new UriFormatException(SR.net_uri_CannotCreateRelative);
                default:
                    break;
            }
            return new UriFormatException(SR.net_uri_BadFormat);
        }

        public string AbsolutePath
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                string path = PrivateAbsolutePath;
                //
                // For Compat:
                // Remove the first slash from a Dos Path if it's present
                //
                if (IsDosPath && path[0] == '/')
                {
                    path = path.Substring(1);
                }
                return path;
            }
        }

        private string PrivateAbsolutePath
        {
            get
            {
                Debug.Assert(IsAbsoluteUri);

                MoreInfo info = EnsureUriInfo().MoreInfo;
                return info.Path ??= GetParts(UriComponents.Path | UriComponents.KeepDelimiter, UriFormat.UriEscaped);
            }
        }

        public string AbsoluteUri
        {
            get
            {
                if (_syntax == null)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                MoreInfo info = EnsureUriInfo().MoreInfo;
                return info.AbsoluteUri ??= GetParts(UriComponents.AbsoluteUri, UriFormat.UriEscaped);
            }
        }

        //
        // LocalPath
        //
        //  Returns a 'local' version of the path. This is mainly for file: URI
        //  such that DOS and UNC paths are returned with '/' converted back to
        //  '\', and any escape sequences converted
        //
        //  The form of the returned path is in NOT Escaped
        //
        public string LocalPath
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }
                return GetLocalPath();
            }
        }

        //
        // The result is of the form "hostname[:port]" Port is omitted if default
        //
        public string Authority
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                // Note: Compatibility with V1 that does not report user info
                return GetParts(UriComponents.Host | UriComponents.Port, UriFormat.UriEscaped);
            }
        }


        public UriHostNameType HostNameType
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                if (_syntax.IsSimple)
                    EnsureUriInfo();
                else
                {
                    // For a custom parser we request HostString creation to confirm HostType
                    EnsureHostString(false);
                }

                switch (HostType)
                {
                    case Flags.DnsHostType: return UriHostNameType.Dns;
                    case Flags.IPv4HostType: return UriHostNameType.IPv4;
                    case Flags.IPv6HostType: return UriHostNameType.IPv6;
                    case Flags.BasicHostType: return UriHostNameType.Basic;
                    case Flags.UncHostType: return UriHostNameType.Basic;
                    case Flags.UnknownHostType: return UriHostNameType.Unknown;
                    default:
                        break;
                }
                return UriHostNameType.Unknown;
            }
        }

        public bool IsDefaultPort
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }
                if (_syntax.IsSimple)
                    EnsureUriInfo();
                else
                {
                    // For a custom parser we request HostString creation that will also set the port
                    EnsureHostString(false);
                }

                return NotAny(Flags.NotDefaultPort);
            }
        }

        public bool IsFile
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                return (object)_syntax.SchemeName == (object)UriSchemeFile;
            }
        }

        public bool IsLoopback
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                EnsureHostString(false);

                return InFact(Flags.LoopbackHost);
            }
        }

        //
        //  Gets the escaped Uri.AbsolutePath and Uri.Query
        //  properties separated by a "?" character.
        public string PathAndQuery
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                UriInfo info = EnsureUriInfo();

                if (info.PathAndQuery is null)
                {
                    string result = GetParts(UriComponents.PathAndQuery, UriFormat.UriEscaped);

                    // Compatibility:
                    // Remove the first slash from a Dos Path if it's present
                    if (IsDosPath && result[0] == '/')
                    {
                        result = result.Substring(1);
                    }

                    info.PathAndQuery = result;
                }

                return info.PathAndQuery;
            }
        }

        //
        //  Gets an array of the segments that make up a URI.
        public string[] Segments
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                string[] segments;
                string path = PrivateAbsolutePath;

                if (path.Length == 0)
                {
                    segments = Array.Empty<string>();
                }
                else
                {
                    ArrayBuilder<string> pathSegments = default;
                    int current = 0;
                    while (current < path.Length)
                    {
                        int next = path.IndexOf('/', current);
                        if (next == -1)
                        {
                            next = path.Length - 1;
                        }
                        pathSegments.Add(path.Substring(current, (next - current) + 1));
                        current = next + 1;
                    }
                    segments = pathSegments.ToArray();
                }

                return segments;
            }
        }

        public bool IsUnc
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }
                return IsUncPath;
            }
        }

        //
        // Gets a hostname part (special formatting for IPv6 form)
        public string Host
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                return GetParts(UriComponents.Host, UriFormat.UriEscaped);
            }
        }

        private static bool StaticIsFile(UriParser syntax)
        {
            return syntax.InFact(UriSyntaxFlags.FileLikeUri);
        }

        private string GetLocalPath()
        {
            EnsureParseRemaining();

            //Other cases will get a Unix-style path
            if (IsUncOrDosPath)
            {
                EnsureHostString(false);
                Debug.Assert(_info != null);
                Debug.Assert(_info.Host != null);
                int start;

                // Do we have a valid local path right in _string?
                if (NotAny(Flags.HostNotCanonical | Flags.PathNotCanonical | Flags.ShouldBeCompressed))
                {
                    start = IsUncPath ? _info.Offset.Host - 2 : _info.Offset.Path;

                    string str = (IsImplicitFile && _info.Offset.Host == (IsDosPath ? 0 : 2) &&
                        _info.Offset.Query == _info.Offset.End)
                            ? _string
                            : (IsDosPath && (_string[start] == '/' || _string[start] == '\\'))
                                ? _string.Substring(start + 1, _info.Offset.Query - start - 1)
                                : _string.Substring(start, _info.Offset.Query - start);

                    // Should be a rare case, convert c|\ into c:\
                    if (IsDosPath && str[1] == '|')
                    {
                        // Sadly, today there is no method for replacing just one occurrence
                        str = str.Remove(1, 1);
                        str = str.Insert(1, ":");
                    }

                    // check for all back slashes
                    str = str.Replace('/', '\\');

                    return str;
                }

                char[] result;
                int count = 0;
                start = _info.Offset.Path;

                string host = _info.Host;
                result = new char[host.Length + 3 + _info.Offset.Fragment - _info.Offset.Path];

                if (IsUncPath)
                {
                    result[0] = '\\';
                    result[1] = '\\';
                    count = 2;

                    UriHelper.UnescapeString(host, 0, host.Length, result, ref count, c_DummyChar, c_DummyChar,
                        c_DummyChar, UnescapeMode.CopyOnly, _syntax, false);
                }
                else
                {
                    // Dos path
                    if (_string[start] == '/' || _string[start] == '\\')
                    {
                        // Skip leading slash for a DOS path
                        ++start;
                    }
                }


                int pathStart = count; // save for optional Compress() call

                UnescapeMode mode = (InFact(Flags.PathNotCanonical) && !IsImplicitFile)
                    ? (UnescapeMode.Unescape | UnescapeMode.UnescapeAll) : UnescapeMode.CopyOnly;
                UriHelper.UnescapeString(_string, start, _info.Offset.Query, result, ref count, c_DummyChar,
                    c_DummyChar, c_DummyChar, mode, _syntax, true);

                // Possibly convert c|\ into c:\
                if (result[1] == '|')
                    result[1] = ':';

                if (InFact(Flags.ShouldBeCompressed))
                {
                    // suspecting not compressed path
                    // For a dos path we won't compress the "x:" part if found /../ sequences
                    Compress(result, IsDosPath ? pathStart + 2 : pathStart, ref count, _syntax);
                }

                // We don't know whether all slashes were the back ones
                // Plus going through Compress will turn them into / anyway
                // Converting / back into \
                Span<char> slashSpan = result.AsSpan(0, count);
                slashSpan.Replace('/', '\\');

                return new string(result, 0, count);
            }
            else
            {
                // Return unescaped canonical path
                // Note we cannot call GetParts here because it has circular dependency on GelLocalPath method
                return GetUnescapedParts(UriComponents.Path | UriComponents.KeepDelimiter, UriFormat.Unescaped);
            }
        }

        public int Port
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                if (_syntax.IsSimple)
                    EnsureUriInfo();
                else
                {
                    // For a custom parser we request HostString creation that will also set the port
                    EnsureHostString(false);
                }

                if (InFact(Flags.NotDefaultPort))
                {
                    return (int)_info.Offset.PortValue;
                }
                return _syntax.DefaultPort;
            }
        }

        //
        //  Gets the escaped query.
        public string Query
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                MoreInfo info = EnsureUriInfo().MoreInfo;
                return info.Query ??= GetParts(UriComponents.Query | UriComponents.KeepDelimiter, UriFormat.UriEscaped);
            }
        }

        //
        //    Gets the escaped fragment.
        public string Fragment
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                MoreInfo info = EnsureUriInfo().MoreInfo;
                return info.Fragment ??= GetParts(UriComponents.Fragment | UriComponents.KeepDelimiter, UriFormat.UriEscaped);
            }
        }

        //
        //  Gets the Scheme string of this Uri
        //
        public string Scheme
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                return _syntax.SchemeName;
            }
        }

        //
        //  Gets the exact string passed by a user.
        //  The original string will switched from _string to _originalUnicodeString if
        //  iri is turned on and we have non-ascii chars
        //
        public string OriginalString => _originalUnicodeString ?? _string;

        //
        //    Gets the host string that is unescaped and if it's Ipv6 host,
        //    then the returned string is suitable for DNS lookup.
        //
        //    For Ipv6 this will strip [] and add ScopeId if was found in the original string
        public string DnsSafeHost
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                EnsureHostString(false);

                Flags hostType = HostType;
                if (hostType == Flags.IPv6HostType || (hostType == Flags.BasicHostType && InFact(Flags.HostNotCanonical | Flags.E_HostNotCanonical)))
                {
                    return IdnHost;
                }
                else
                {
                    return _info.Host!;
                }
            }
        }

        // Returns the host name represented as IDN (using punycode encoding) regardless of app.config settings
        public string IdnHost
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                if (_info?.IdnHost is null)
                {
                    EnsureHostString(false);

                    string host = _info.Host!;

                    Flags hostType = HostType;
                    if (hostType == Flags.DnsHostType)
                    {
                        host = DomainNameHelper.IdnEquivalent(host);
                    }
                    else if (hostType == Flags.IPv6HostType)
                    {
                        host = _info._moreInfo?.ScopeId is string scopeId ?
                            string.Concat(host.AsSpan(1, host.Length - 2), scopeId) :
                            host.Substring(1, host.Length - 2);
                    }
                    // Validate that this basic host qualifies as Dns safe,
                    // It has looser parsing rules that might allow otherwise.
                    // It might be a registry-based host from RFC 2396 Section 3.2.1
                    else if (hostType == Flags.BasicHostType && InFact(Flags.HostNotCanonical | Flags.E_HostNotCanonical))
                    {
                        // Unescape everything
                        var dest = new ValueStringBuilder(stackalloc char[StackallocThreshold]);

                        UriHelper.UnescapeString(host, 0, host.Length, ref dest,
                            c_DummyChar, c_DummyChar, c_DummyChar,
                            UnescapeMode.Unescape | UnescapeMode.UnescapeAll,
                            _syntax, isQuery: false);

                        host = dest.ToString();
                    }

                    _info.IdnHost = host;
                }

                return _info.IdnHost;
            }
        }

        //
        //  Returns false if the string passed in the constructor cannot be parsed as
        //  valid AbsoluteUri. This could be a relative Uri instead.
        //
        public bool IsAbsoluteUri
        {
            get
            {
                return _syntax != null;
            }
        }

        //
        //  Returns 'true' if the 'dontEscape' parameter was set to 'true ' when the Uri instance was created.
        public bool UserEscaped
        {
            get
            {
                return InFact(Flags.UserEscaped);
            }
        }

        //
        //  Gets the user name, password, and other user specific information associated
        //  with the Uniform Resource Identifier (URI).
        public string UserInfo
        {
            get
            {
                if (IsNotAbsoluteUri)
                {
                    throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                }

                return GetParts(UriComponents.UserInfo, UriFormat.UriEscaped);
            }
        }

        //
        // CheckHostName
        //
        //  Determines whether a host name authority is a valid Host name according
        //  to DNS naming rules and IPv4 canonicalization rules
        //
        // Returns:
        //  true if <name> is valid else false
        //
        // Throws:
        //  Nothing
        //
        public static UriHostNameType CheckHostName(string? name)
        {
            if (string.IsNullOrEmpty(name))
            {
                return UriHostNameType.Unknown;
            }

            int end = name.Length;
            unsafe
            {
                fixed (char* fixedName = name)
                {
                    if (name.StartsWith('[') && name.EndsWith(']'))
                    {
                        // we require that _entire_ name is recognized as ipv6 address
                        if (IPv6AddressHelper.IsValid(fixedName, 1, ref end) && end == name.Length)
                        {
                            return UriHostNameType.IPv6;
                        }
                    }

                    end = name.Length;
                    if (IPv4AddressHelper.IsValid(fixedName, 0, ref end, false, false, false) && end == name.Length)
                    {
                        return UriHostNameType.IPv4;
                    }
                }

                if (DomainNameHelper.IsValid(name, iri: false, notImplicitFile: false, out int length) && length == name.Length)
                {
                    return UriHostNameType.Dns;
                }

                if (DomainNameHelper.IsValid(name, iri: true, notImplicitFile: false, out length) && length == name.Length)
                {
                    return UriHostNameType.Dns;
                }

                //This checks the form without []
                end = name.Length + 2;
                // we require that _entire_ name is recognized as ipv6 address
                name = "[" + name + "]";
                fixed (char* newFixedName = name)
                {
                    if (IPv6AddressHelper.IsValid(newFixedName, 1, ref end) && end == name.Length)
                    {
                        return UriHostNameType.IPv6;
                    }
                }
            }
            return UriHostNameType.Unknown;
        }

        //
        // GetLeftPart
        //
        //  Returns part of the URI based on the parameters:
        //
        // Inputs:
        //  <argument>  part
        //      Which part of the URI to return
        //
        // Returns:
        //  The requested substring
        //
        // Throws:
        //  UriFormatException if URI type doesn't have host-port or authority parts
        //
        public string GetLeftPart(UriPartial part)
        {
            if (IsNotAbsoluteUri)
            {
                throw new InvalidOperationException(SR.net_uri_NotAbsolute);
            }

            EnsureUriInfo();
            const UriComponents NonPathPart = (UriComponents.Scheme | UriComponents.UserInfo | UriComponents.Host | UriComponents.Port);

            switch (part)
            {
                case UriPartial.Scheme:

                    return GetParts(UriComponents.Scheme | UriComponents.KeepDelimiter, UriFormat.UriEscaped);

                case UriPartial.Authority:

                    if (NotAny(Flags.AuthorityFound) || IsDosPath)
                    {
                        // V1.0 compatibility.
                        // It not return an empty string but instead "scheme:" because it is a LEFT part.
                        // Also neither it should check for IsDosPath here

                        // From V1.0 comments:

                        // anything that didn't have "//" after the scheme name
                        // (mailto: and news: e.g.) doesn't have an authority
                        //

                        return string.Empty;
                    }
                    return GetParts(NonPathPart, UriFormat.UriEscaped);

                case UriPartial.Path:
                    return GetParts(NonPathPart | UriComponents.Path, UriFormat.UriEscaped);

                case UriPartial.Query:
                    return GetParts(NonPathPart | UriComponents.Path | UriComponents.Query, UriFormat.UriEscaped);
            }
            throw new ArgumentException(SR.Format(SR.Argument_InvalidUriSubcomponent, part), nameof(part));
        }

        //
        //
        /// Transforms a character into its hexadecimal representation.
        public static string HexEscape(char character)
        {
            ArgumentOutOfRangeException.ThrowIfGreaterThan(character, '\xff');

            return string.Create(3, (byte)character, (Span<char> chars, byte b) =>
            {
                chars[0] = '%';
                HexConverter.ToCharsBuffer(b, chars, 1, HexConverter.Casing.Upper);
            });
        }

        //
        // HexUnescape
        //
        //  Converts a substring of the form "%XX" to the single character represented
        //  by the hexadecimal value XX. If the substring s[Index] does not conform to
        //  the hex encoding format then the character at s[Index] is returned
        //
        // Inputs:
        //  <argument>  pattern
        //      String from which to read the hexadecimal encoded substring
        //
        //  <argument>  index
        //      Offset within <pattern> from which to start reading the hexadecimal
        //      encoded substring
        //
        // Outputs:
        //  <argument>  index
        //      Incremented to the next character position within the string. This
        //      may be EOS if this was the last character/encoding within <pattern>
        //
        // Returns:
        //  Either the converted character if <pattern>[<index>] was hex encoded, or
        //  the character at <pattern>[<index>]
        //
        // Throws:
        //  ArgumentOutOfRangeException
        //

        public static char HexUnescape(string pattern, ref int index)
        {
            ArgumentOutOfRangeException.ThrowIfNegative(index);
            ArgumentOutOfRangeException.ThrowIfGreaterThanOrEqual(index, pattern.Length);

            if ((pattern[index] == '%')
                && (pattern.Length - index >= 3))
            {
                char ret = UriHelper.DecodeHexChars(pattern[index + 1], pattern[index + 2]);
                if (ret != c_DummyChar)
                {
                    index += 3;
                    return ret;
                }
            }
            return pattern[index++];
        }

        //
        // IsHexEncoding
        //
        //  Determines whether a substring has the URI hex encoding format of '%'
        //  followed by 2 hexadecimal characters
        //
        // Inputs:
        //  <argument>  pattern
        //      String to check
        //
        //  <argument>  index
        //      Offset in <pattern> at which to check substring for hex encoding
        //
        // Assumes:
        //  0 <= <index> < <pattern>.Length
        //
        // Returns:
        //  true if <pattern>[<index>] is hex encoded, else false
        //
        // Throws:
        //  Nothing
        //
        public static bool IsHexEncoding(string pattern, int index)
        {
            return
                (pattern.Length - index) >= 3 &&
                pattern[index] == '%' &&
                char.IsAsciiHexDigit(pattern[index + 1]) &&
                char.IsAsciiHexDigit(pattern[index + 2]);
        }

        private static readonly SearchValues<char> s_schemeChars =
            SearchValues.Create("+-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

        // CheckSchemeName
        //
        //  Determines whether a string is a valid scheme name according to RFC 2396.
        //  Syntax is:
        //      scheme = alpha *(alpha | digit | '+' | '-' | '.')
        public static bool CheckSchemeName([NotNullWhen(true)] string? schemeName) =>
            !string.IsNullOrEmpty(schemeName) &&
            char.IsAsciiLetter(schemeName[0]) &&
            !schemeName.AsSpan().ContainsAnyExcept(s_schemeChars);

        //
        // IsHexDigit
        //
        //  Determines whether a character is a valid hexadecimal digit in the range
        //  [0..9] | [A..F] | [a..f]
        //
        // Inputs:
        //  <argument>  character
        //      Character to test
        //
        // Returns:
        //  true if <character> is a hexadecimal digit character
        //
        // Throws:
        //  Nothing
        //
        public static bool IsHexDigit(char character)
        {
            return char.IsAsciiHexDigit(character);
        }

        //
        // Returns:
        //  Number in the range 0..15
        //
        // Throws:
        //  ArgumentException
        //
        public static int FromHex(char digit)
        {
            int result = HexConverter.FromChar(digit);
            if (result == 0xFF)
            {
                throw new ArgumentException(null, nameof(digit));
            }

            return result;
        }

        public override int GetHashCode()
        {
            if (IsNotAbsoluteUri)
            {
                return OriginalString.GetHashCode();
            }
            else
            {
                MoreInfo info = EnsureUriInfo().MoreInfo;

                UriComponents components = UriComponents.HttpRequestUrl;

                if (_syntax.InFact(UriSyntaxFlags.MailToLikeUri))
                {
                    components |= UriComponents.UserInfo;
                }

                string remoteUrl = info.RemoteUrl ??= GetParts(components, UriFormat.SafeUnescaped);

                if (IsUncOrDosPath)
                {
                    return StringComparer.OrdinalIgnoreCase.GetHashCode(remoteUrl);
                }
                else
                {
                    return remoteUrl.GetHashCode();
                }
            }
        }

        //
        // ToString
        //
        private const UriFormat V1ToStringUnescape = (UriFormat)0x7FFF;

        public override string ToString()
        {
            if (_syntax == null)
            {
                return _string;
            }

            EnsureUriInfo();
            return _info.String ??=
                _syntax.IsSimple ?
                    GetComponentsHelper(UriComponents.AbsoluteUri, V1ToStringUnescape) :
                    GetParts(UriComponents.AbsoluteUri, UriFormat.SafeUnescaped);
        }

        /// <summary>
        /// Attempts to format a canonical string representation for the <see cref="Uri"/> instance into the specified span.
        /// </summary>
        /// <param name="destination">The span into which to write this instance's value formatted as a span of characters.</param>
        /// <param name="charsWritten">When this method returns, contains the number of characters that were written in <paramref name="destination"/>.</param>
        /// <returns><see langword="true"/> if the formatting was successful; otherwise, <see langword="false"/>.</returns>
        public bool TryFormat(Span<char> destination, out int charsWritten)
        {
            ReadOnlySpan<char> result;

            if (_syntax is null)
            {
                result = _string;
            }
            else
            {
                EnsureUriInfo();
                if (_info.String is not null)
                {
                    result = _info.String;
                }
                else
                {
                    UriFormat uriFormat = V1ToStringUnescape;
                    if (!_syntax.IsSimple)
                    {
                        if (IsNotAbsoluteUri)
                        {
                            throw new InvalidOperationException(SR.net_uri_NotAbsolute);
                        }

                        if (UserDrivenParsing)
                        {
                            throw new InvalidOperationException(SR.Format(SR.net_uri_UserDrivenParsing, GetType()));
                        }

                        if (DisablePathAndQueryCanonicalization)
                        {
                            throw new InvalidOperationException(SR.net_uri_GetComponentsCalledWhenCanonicalizationDisabled);
                        }

                        uriFormat = UriFormat.SafeUnescaped;
                    }

                    EnsureParseRemaining();
                    EnsureHostString(allowDnsOptimization: true);

                    ushort nonCanonical = (ushort)((ushort)_flags & (ushort)Flags.CannotDisplayCanonical);
                    if (((_flags & (Flags.ShouldBeCompressed | Flags.FirstSlashAbsent | Flags.BackslashInPath)) != 0) ||
                        (IsDosPath && _string[_info.Offset.Path + SecuredPathIndex - 1] == '|')) // A rare case of c|\
                    {
                        nonCanonical |= (ushort)Flags.PathNotCanonical;
                    }

                    if (((ushort)UriComponents.AbsoluteUri & nonCanonical) != 0)
                    {
                        return TryRecreateParts(destination, out charsWritten, UriComponents.AbsoluteUri, nonCanonical, uriFormat);
                    }

                    result = _string.AsSpan(_info.Offset.Scheme, _info.Offset.End - _info.Offset.Scheme);
                }
            }

            if (result.TryCopyTo(destination))
            {
                charsWritten = result.Length;
                return true;
            }

            charsWritten = 0;
            return false;
        }

        /// <inheritdoc/>
        bool ISpanFormattable.TryFormat(Span<char> destination, out int charsWritten, ReadOnlySpan<char> format, IFormatProvider? provider) =>
            TryFormat(destination, out charsWritten);

        /// <inheritdoc/>
        string IFormattable.ToString(string? format, IFormatProvider? formatProvider) =>
            ToString();

        public static bool operator ==(Uri? uri1, Uri? uri2)
        {
            if (ReferenceEquals(uri1, uri2))
            {
                return true;
            }

            if (uri1 is null || uri2 is null)
            {
                return false;
            }

            return uri1.Equals(uri2);
        }

        public static bool operator !=(Uri? uri1, Uri? uri2)
        {
            if (ReferenceEquals(uri1, uri2))
            {
                return false;
            }

            if (uri1 is null || uri2 is null)
            {
                return true;
            }

            return !uri1.Equals(uri2);
        }

        public override bool Equals([NotNullWhen(true)] object? comparand)
        {
            if (comparand is null)
            {
                return false;
            }

            if (ReferenceEquals(this, comparand))
            {
                return true;
            }

            Uri? other = comparand as Uri;

            // we allow comparisons of Uri and String objects only. If a string
            // is passed, convert to Uri. This is inefficient, but allows us to
            // canonicalize the comparand, making comparison possible
            if (other is null)
            {
                if (DisablePathAndQueryCanonicalization)
                    return false;

                if (!(comparand is string s))
                    return false;

                if (ReferenceEquals(s, OriginalString))
                    return true;

                if (!TryCreate(s, UriKind.RelativeOrAbsolute, out other))
                    return false;
            }

            return Equals(other);
        }

        /// <summary>
        /// Compares two <see cref="Uri"/> instances for equality.
        /// </summary>
        /// <param name="other">The <see cref="Uri"/> to compare to this instance.</param>
        /// <returns><see langword="true"/> if the two instances represent the same URI; otherwise, <see langword="false"/>.</returns>
        public bool Equals([NotNullWhen(true)] Uri? other)
        {
            if (other is null)
            {
                return false;
            }

            if (ReferenceEquals(this, other))
            {
                return true;
            }

            if (DisablePathAndQueryCanonicalization != other.DisablePathAndQueryCanonicalization)
                return false;

            if (ReferenceEquals(OriginalString, other.OriginalString))
            {
                return true;
            }

            if (IsAbsoluteUri != other.IsAbsoluteUri)
                return false;

            if (IsNotAbsoluteUri)
                return OriginalString.Equals(other.OriginalString);

            if (NotAny(Flags.AllUriInfoSet) || other.NotAny(Flags.AllUriInfoSet))
            {
                // Try raw compare for _strings as the last chance to keep the working set small
                if (string.Equals(_string, other._string, IsUncOrDosPath ? StringComparison.OrdinalIgnoreCase : StringComparison.Ordinal))
                {
                    return true;
                }
            }

            // Note that equality test will bring the working set of both
            // objects up to creation of _info.MoreInfo member
            EnsureUriInfo();
            other.EnsureUriInfo();

            if (!UserDrivenParsing && !other.UserDrivenParsing && Syntax!.IsSimple && other.Syntax.IsSimple)
            {
                // Optimization of canonical DNS names by avoiding host string creation.
                // Note there could be explicit ports specified that would invalidate path offsets
                if (InFact(Flags.CanonicalDnsHost) && other.InFact(Flags.CanonicalDnsHost))
                {
                    int i1 = _info.Offset.Host;
                    int end1 = _info.Offset.Path;

                    int i2 = other._info.Offset.Host;
                    int end2 = other._info.Offset.Path;
                    string str = other._string;
                    //Taking the shortest part
                    if (end1 - i1 > end2 - i2)
                    {
                        end1 = i1 + end2 - i2;
                    }
                    // compare and break on ':' if found
                    while (i1 < end1)
                    {
                        if (_string[i1] != str[i2])
                        {
                            return false;
                        }
                        if (str[i2] == ':')
                        {
                            // The other must have ':' too to have equal host
                            break;
                        }
                        ++i1; ++i2;
                    }

                    // The longest host must have ':' or be of the same size
                    if (i1 < _info.Offset.Path && _string[i1] != ':')
                    {
                        return false;
                    }
                    if (i2 < end2 && str[i2] != ':')
                    {
                        return false;
                    }
                    //hosts are equal!
                }
                else
                {
                    EnsureHostString(false);
                    other.EnsureHostString(false);
                    if (!_info.Host!.Equals(other._info.Host))
                    {
                        return false;
                    }
                }

                if (Port != other.Port)
                {
                    return false;
                }
            }

            // We want to cache RemoteUrl to improve perf for Uri as a key.
            // We should consider reducing the overall working set by not caching some other properties mentioned in MoreInfo

            MoreInfo selfInfo = _info.MoreInfo;
            MoreInfo otherInfo = other._info.MoreInfo;

            // Fragment AND UserInfo (for non-mailto URIs) are ignored
            UriComponents components = UriComponents.HttpRequestUrl;

            if (_syntax.InFact(UriSyntaxFlags.MailToLikeUri))
            {
                if (!other._syntax.InFact(UriSyntaxFlags.MailToLikeUri))
                    return false;

                components |= UriComponents.UserInfo;
            }

            string selfUrl = selfInfo.RemoteUrl ??= GetParts(components, UriFormat.SafeUnescaped);
            string otherUrl = otherInfo.RemoteUrl ??= other.GetParts(components, UriFormat.SafeUnescaped);

            // if IsUncOrDosPath is true then we ignore case in the path comparison
            return string.Equals(selfUrl, otherUrl, IsUncOrDosPath ? StringComparison.OrdinalIgnoreCase : StringComparison.Ordinal);
        }

        public Uri MakeRelativeUri(Uri uri)
        {
            ArgumentNullException.ThrowIfNull(uri);

            if (IsNotAbsoluteUri || uri.IsNotAbsoluteUri)
                throw new InvalidOperationException(SR.net_uri_NotAbsolute);

            // Note that the UserInfo part is ignored when computing a relative Uri.
            if ((Scheme == uri.Scheme) && (Host == uri.Host) && (Port == uri.Port))
            {
                string otherPath = uri.AbsolutePath;

                // Relative Path
                string relativeUriString = PathDifference(AbsolutePath, otherPath, !IsUncOrDosPath);

                // Relative Uri's cannot have a colon ':' in the first path segment (RFC 3986, Section 4.2)
                if (CheckForColonInFirstPathSegment(relativeUriString)
                    // Except for full implicit dos file paths
                    && !(uri.IsDosPath && otherPath.Equals(relativeUriString, StringComparison.Ordinal)))
                    relativeUriString = "./" + relativeUriString;

                // Query & Fragment
                relativeUriString += uri.GetParts(UriComponents.Query | UriComponents.Fragment, UriFormat.UriEscaped);

                return new Uri(relativeUriString, UriKind.Relative);
            }
            return uri;
        }

        //
        // http://www.ietf.org/rfc/rfc3986.txt
        //
        // 3.3.  Path
        // In addition, a URI reference (Section 4.1) may be a relative-path reference, in which case the  first
        // path segment cannot contain a colon (":") character.
        //
        // 4.2.  Relative Reference
        // A path segment that contains a colon character (e.g., "this:that") cannot be used as the first segment
        // of a relative-path reference, as it would be mistaken for a scheme name.  Such a segment must be
        // preceded by a dot-segment (e.g., "./this:that") to make a relative-path reference.
        //
        // 5.4.2. Abnormal Examples
        // http:(relativeUri) may be considered a valid relative Uri.
        //
        // Returns true if a colon is found in the first path segment, false otherwise
        //
        private static readonly SearchValues<char> s_segmentSeparatorChars =
            SearchValues.Create(@":\/?#");

        private static bool CheckForColonInFirstPathSegment(string uriString)
        {
            // Check for anything that may terminate the first regular path segment
            // or an illegal colon
            int index = uriString.AsSpan().IndexOfAny(s_segmentSeparatorChars);
            return (uint)index < (uint)uriString.Length && uriString[index] == ':';
        }

        internal static string InternalEscapeString(string rawString) =>
            rawString is null ? string.Empty :
            UriHelper.EscapeString(rawString, checkExistingEscaped: true, UriHelper.UnreservedReservedExceptQuestionMarkHash);

        //
        //  This method is called first to figure out the scheme or a simple file path
        //  Is called only at the .ctor time
        //
        private static ParsingError ParseScheme(string uriString, ref Flags flags, ref UriParser? syntax)
        {
            Debug.Assert((flags & Flags.Debug_LeftConstructor) == 0);

            int length = uriString.Length;
            if (length == 0)
                return ParsingError.EmptyUriString;

            // Fast path for valid http(s) schemes with no leading whitespace that are expected to be very common.
            if (uriString.StartsWith("https:", StringComparison.OrdinalIgnoreCase))
            {
                syntax = UriParser.HttpsUri;
                flags |= (Flags)6;
            }
            else if (uriString.StartsWith("http:", StringComparison.OrdinalIgnoreCase))
            {
                syntax = UriParser.HttpUri;
                flags |= (Flags)5;
            }
            else
            {
                // STEP1: parse scheme, lookup this Uri Syntax or create one using UnknownV1SyntaxFlags uri syntax template
                ParsingError err = ParsingError.None;
                int idx = ParseSchemeCheckImplicitFile(uriString, ref err, ref flags, ref syntax);
                Debug.Assert((err is ParsingError.None) == (syntax is not null));

                if (err != ParsingError.None)
                    return err;

                flags |= (Flags)idx;
            }

            return ParsingError.None;
        }

        //
        // A wrapper for ParseMinimal() called from a user parser
        // It signals back that the call has been done
        // plus it communicates back a flag for an error if any
        //
        internal UriFormatException? ParseMinimal()
        {
            Debug.Assert(_syntax != null && !_syntax.IsSimple);
            Debug.Assert((_flags & Flags.CustomParser_ParseMinimalAlreadyCalled) != 0);
            DebugAssertInCtor();

            ParsingError result = PrivateParseMinimal();
            if (result == ParsingError.None)
                return null;

            // Means the we think the Uri is invalid, bu that can be later overridden by a user parser
            _flags |= Flags.ErrorOrParsingRecursion;

            return GetException(result);
        }

        //
        //
        //  This method tries to parse the minimal information needed to certify the validity
        //  of a uri string
        //
        //      scheme://userinfo@host:Port/Path?Query#Fragment
        //
        //  The method must be called only at the .ctor time
        //
        //  Returns ParsingError.None if the Uri syntax is valid, an error otherwise
        //
        private unsafe ParsingError PrivateParseMinimal()
        {
            Debug.Assert(_syntax != null);
            DebugAssertInCtor();

            int idx = (int)(_flags & Flags.IndexMask);
            int length = _string.Length;

            // Means a custom UriParser did call "base" InitializeAndValidate()
            _flags &= ~(Flags.IndexMask | Flags.UserDrivenParsing);

            //STEP2: Parse up to the port

            Debug.Assert(ReferenceEquals(_string, OriginalString));

            fixed (char* pUriString = _string)
            {
                // Cut trailing spaces in _string
                if (length > idx && UriHelper.IsLWS(pUriString[length - 1]))
                {
                    --length;
                    while (length != idx && UriHelper.IsLWS(pUriString[--length]))
                        ;
                    ++length;
                }

                // Unix Path
                if (!OperatingSystem.IsWindows() && InFact(Flags.UnixPath))
                {
                    _flags |= Flags.BasicHostType;
                    _flags |= (Flags)idx;
                    return ParsingError.None;
                }

                // Old Uri parser tries to figure out on a DosPath in all cases.
                // Hence http://c:/ is treated as DosPath without the host while it should be a host "c", port 80
                //
                // This block is compatible with Old Uri parser in terms it will look for the DosPath if the scheme
                // syntax allows both empty hostnames and DosPath
                //
                if (_syntax.IsAllSet(UriSyntaxFlags.AllowEmptyHost | UriSyntaxFlags.AllowDOSPath)
                    && NotAny(Flags.ImplicitFile) && (idx + 1 < length))
                {
                    char c;
                    int i = idx;

                    // V1 Compat: Allow _compression_ of > 3 slashes only for File scheme.
                    // This will skip all slashes and if their number is 2+ it sets the AuthorityFound flag
                    for (; i < length; ++i)
                    {
                        if (!((c = pUriString[i]) == '\\' || c == '/'))
                            break;
                    }

                    if (_syntax.InFact(UriSyntaxFlags.FileLikeUri) || i - idx <= 3)
                    {
                        // if more than one slash after the scheme, the authority is present
                        if (i - idx >= 2)
                        {
                            _flags |= Flags.AuthorityFound;
                        }
                        // DOS-like path?
                        if (i + 1 < length && ((c = pUriString[i + 1]) == ':' || c == '|') &&
                            char.IsAsciiLetter(pUriString[i]))
                        {
                            if (i + 2 >= length || ((c = pUriString[i + 2]) != '\\' && c != '/'))
                            {
                                // report an error but only for a file: scheme
                                if (_syntax.InFact(UriSyntaxFlags.FileLikeUri))
                                    return ParsingError.MustRootedPath;
                            }
                            else
                            {
                                // This will set IsDosPath
                                _flags |= Flags.DosPath;

                                if (_syntax.InFact(UriSyntaxFlags.MustHaveAuthority))
                                {
                                    // when DosPath found and Authority is required, set this flag even if Authority is empty
                                    _flags |= Flags.AuthorityFound;
                                }
                                if (i != idx && i - idx != 2)
                                {
                                    //This will remember that DosPath is rooted
                                    idx = i - 1;
                                }
                                else
                                {
                                    idx = i;
                                }
                            }
                        }
                        // UNC share?
                        else if (_syntax.InFact(UriSyntaxFlags.FileLikeUri) && (i - idx >= 2 && i - idx != 3 &&
                            i < length && pUriString[i] != '?' && pUriString[i] != '#'))
                        {
                            // V1.0 did not support file:///, fixing it with minimal behavior change impact
                            // Only FILE scheme may have UNC Path flag set
                            _flags |= Flags.UncPath;
                            idx = i;
                        }
                        else if (!OperatingSystem.IsWindows() && _syntax.InFact(UriSyntaxFlags.FileLikeUri) && pUriString[i - 1] == '/' && i - idx == 3)
                        {
                            _syntax = UriParser.UnixFileUri;
                            _flags |= Flags.UnixPath | Flags.AuthorityFound;
                            idx += 2;
                        }
                    }
                }
                //
                //STEP 1.5 decide on the Authority component
                //
                if ((_flags & (Flags.UncPath | Flags.DosPath | Flags.UnixPath)) != 0)
                {
                }
                else if ((idx + 2) <= length)
                {
                    char first = pUriString[idx];
                    char second = pUriString[idx + 1];

                    if (_syntax.InFact(UriSyntaxFlags.MustHaveAuthority))
                    {
                        // (V1.0 compatibility) This will allow http:\\ http:\/ http:/\
                        if ((first == '/' || first == '\\') && (second == '/' || second == '\\'))
                        {
                            _flags |= Flags.AuthorityFound;
                            idx += 2;
                        }
                        else
                        {
                            return ParsingError.BadAuthority;
                        }
                    }
                    else if (_syntax.InFact(UriSyntaxFlags.OptionalAuthority) && (InFact(Flags.AuthorityFound) ||
                        (first == '/' && second == '/')))
                    {
                        _flags |= Flags.AuthorityFound;
                        idx += 2;
                    }
                    // There is no Authority component, save the Path index
                    // Ideally we would treat mailto like any other URI, but for historical reasons we have to separate out its host parsing.
                    else if (_syntax.NotAny(UriSyntaxFlags.MailToLikeUri))
                    {
                        // By now we know the URI has no Authority, so if the URI must be normalized, initialize it without one.
                        if (InFact(Flags.HasUnicode))
                        {
                            _string = _string.Substring(0, idx);
                        }
                        // Since there is no Authority, the path index is just the end of the scheme.
                        _flags |= ((Flags)idx | Flags.UnknownHostType);
                        return ParsingError.None;
                    }
                }
                else if (_syntax.InFact(UriSyntaxFlags.MustHaveAuthority))
                {
                    return ParsingError.BadAuthority;
                }
                // There is no Authority component, save the Path index
                // Ideally we would treat mailto like any other URI, but for historical reasons we have to separate out its host parsing.
                else if (_syntax.NotAny(UriSyntaxFlags.MailToLikeUri))
                {
                    // By now we know the URI has no Authority, so if the URI must be normalized, initialize it without one.
                    if (InFact(Flags.HasUnicode))
                    {
                        _string = _string.Substring(0, idx);
                    }
                    // Since there is no Authority, the path index is just the end of the scheme.
                    _flags |= ((Flags)idx | Flags.UnknownHostType);
                    return ParsingError.None;
                }

                // vsmacros://c:\path\file
                // Note that two slashes say there must be an Authority but instead the path goes
                // Fro V1 compat the next block allow this case but not for schemes like http
                if (InFact(Flags.DosPath))
                {
                    _flags |= (((_flags & Flags.AuthorityFound) != 0) ? Flags.BasicHostType : Flags.UnknownHostType);
                    _flags |= (Flags)idx;
                    return ParsingError.None;
                }

                //STEP 2: Check the syntax of authority expecting at least one character in it
                //
                // Note here we do know that there is an authority in the string OR it's a DOS path

                // We may find a userInfo and the port when parsing an authority
                // Also we may find a registry based authority.
                // We must ensure that known schemes do use a server-based authority
                {
                    ParsingError err = ParsingError.None;
                    string? newHost = null; // stores newly parsed host when original strings are being switched

                    idx = CheckAuthorityHelper(pUriString, idx, length, ref err, ref _flags, _syntax, ref newHost);
                    if (err != ParsingError.None)
                        return err;

                    if (idx < length)
                    {
                        char hostTerminator = pUriString[idx];

                        // This will disallow '\' as the host terminator for any scheme that is not implicitFile or cannot have a Dos Path
                        if (hostTerminator == '\\' && NotAny(Flags.ImplicitFile) && _syntax.NotAny(UriSyntaxFlags.AllowDOSPath))
                        {
                            return ParsingError.BadAuthorityTerminator;
                        }
                        // When the hostTerminator is '/' on Unix, use the UnixFile syntax (preserve backslashes)
                        else if (!OperatingSystem.IsWindows() && hostTerminator == '/' && NotAny(Flags.ImplicitFile) && InFact(Flags.UncPath) && _syntax == UriParser.FileUri)
                        {
                            _syntax = UriParser.UnixFileUri;
                        }
                    }

                    if (newHost is not null)
                    {
                        _string = newHost;
                    }
                }

                // The Path (or Port) parsing index is reloaded on demand in CreateUriInfo when accessing a Uri property
                _flags |= (Flags)idx;

                // The rest of the string will be parsed on demand
                // The Host/Authority is all checked, the type is known but the host value string
                // is not created/canonicalized at this point.
            }

            return ParsingError.None;
        }

        //
        //
        // The method is called when we have to access _info members.
        // This will create the _info based on the copied parser context.
        // If multi-threading, this method may do duplicated yet harmless work.
        //
        private unsafe void CreateUriInfo(Flags cF)
        {
            UriInfo info = new UriInfo();

            // This will be revisited in ParseRemaining but for now just have it at least _string.Length
            info.Offset.End = _string.Length;

            if (UserDrivenParsing)
                goto Done;

            int idx;
            bool notCanonicalScheme = false;

            // The _string may have leading spaces, figure that out
            // plus it will set idx value for next steps
            if ((cF & Flags.ImplicitFile) != 0)
            {
                idx = 0;
                while (UriHelper.IsLWS(_string[idx]))
                {
                    ++idx;
                    ++info.Offset.Scheme;
                }

                if (StaticInFact(cF, Flags.UncPath))
                {
                    // For implicit file AND Unc only
                    idx += 2;
                    //skip any other slashes (compatibility with V1.0 parser)
                    int end = (int)(cF & Flags.IndexMask);
                    while (idx < end && (_string[idx] == '/' || _string[idx] == '\\'))
                    {
                        ++idx;
                    }
                }
            }
            else
            {
                // This is NOT an ImplicitFile uri
                idx = _syntax.SchemeName.Length;

                while (_string[idx++] != ':')
                {
                    ++info.Offset.Scheme;
                }

                if ((cF & Flags.AuthorityFound) != 0)
                {
                    if (_string[idx] == '\\' || _string[idx + 1] == '\\')
                        notCanonicalScheme = true;

                    idx += 2;
                    if ((cF & (Flags.UncPath | Flags.DosPath)) != 0)
                    {
                        // Skip slashes if it was allowed during ctor time
                        // NB: Today this is only allowed if a Unc or DosPath was found after the scheme
                        int end = (int)(cF & Flags.IndexMask);
                        while (idx < end && (_string[idx] == '/' || _string[idx] == '\\'))
                        {
                            notCanonicalScheme = true;
                            ++idx;
                        }
                    }
                }
            }

            // Some schemes (mailto) do not have Authority-based syntax, still they do have a port
            if (_syntax.DefaultPort != UriParser.NoDefaultPort)
                info.Offset.PortValue = (ushort)_syntax.DefaultPort;

            //Here we set the indexes for already parsed components
            if ((cF & Flags.HostTypeMask) == Flags.UnknownHostType
                || StaticInFact(cF, Flags.DosPath)
                )
            {
                //there is no Authority component defined
                info.Offset.User = (int)(cF & Flags.IndexMask);
                info.Offset.Host = info.Offset.User;
                info.Offset.Path = info.Offset.User;
                cF &= ~Flags.IndexMask;
                if (notCanonicalScheme)
                {
                    cF |= Flags.SchemeNotCanonical;
                }
                goto Done;
            }

            info.Offset.User = idx;

            //Basic Host Type does not have userinfo and port
            if (HostType == Flags.BasicHostType)
            {
                info.Offset.Host = idx;
                info.Offset.Path = (int)(cF & Flags.IndexMask);
                cF &= ~Flags.IndexMask;
                goto Done;
            }

            if ((cF & Flags.HasUserInfo) != 0)
            {
                // we previously found a userinfo, get it again
                while (_string[idx] != '@')
                {
                    ++idx;
                }
                ++idx;
                info.Offset.Host = idx;
            }
            else
            {
                info.Offset.Host = idx;
            }

            //Now reload the end of the parsed host
            idx = (int)(cF & Flags.IndexMask);

            //From now on we do not need IndexMask bits, and reuse the space for X_NotCanonical flags
            //clear them now
            cF &= ~Flags.IndexMask;

            // If this is not canonical, don't count on user input to be good
            if (notCanonicalScheme)
            {
                cF |= Flags.SchemeNotCanonical;
            }

            //Guessing this is a path start
            info.Offset.Path = idx;

            // parse Port if any. The new spec allows a port after ':' to be empty (assuming default?)
            bool notEmpty = false;
            // Note we already checked on general port syntax in ParseMinimal()

            // If iri parsing is on with unicode chars then the end of parsed host
            // points to _originalUnicodeString and not _string

            if ((cF & Flags.HasUnicode) != 0)
                info.Offset.End = _originalUnicodeString.Length;

            if (idx < info.Offset.End)
            {
                fixed (char* userString = OriginalString)
                {
                    if (userString[idx] == ':')
                    {
                        int port = 0;

                        //Check on some non-canonical cases http://host:0324/, http://host:03, http://host:0, etc
                        if (++idx < info.Offset.End)
                        {
                            port = userString[idx] - '0';
                            if ((uint)port <= ('9' - '0'))
                            {
                                notEmpty = true;
                                if (port == 0)
                                {
                                    cF |= (Flags.PortNotCanonical | Flags.E_PortNotCanonical);
                                }
                                for (++idx; idx < info.Offset.End; ++idx)
                                {
                                    int val = userString[idx] - '0';
                                    if ((uint)val > ('9' - '0'))
                                    {
                                        break;
                                    }
                                    port = (port * 10 + val);
                                }
                            }
                        }
                        if (notEmpty && _syntax.DefaultPort != port)
                        {
                            info.Offset.PortValue = (ushort)port;
                            cF |= Flags.NotDefaultPort;
                        }
                        else
                        {
                            //This will tell that we do have a ':' but the port value does
                            //not follow to canonical rules
                            cF |= (Flags.PortNotCanonical | Flags.E_PortNotCanonical);
                        }
                        info.Offset.Path = idx;
                    }
                }
            }

        Done:
            cF |= Flags.MinimalUriInfoSet;

            Interlocked.CompareExchange(ref _info, info, null!);

            Flags current = _flags;
            while ((current & Flags.MinimalUriInfoSet) == 0)
            {
                Flags oldValue = Interlocked.CompareExchange(ref _flags, (current & ~Flags.IndexMask) | cF, current);
                if (oldValue == current)
                {
                    return;
                }
                current = (Flags)oldValue;
            }
        }

        //
        // This will create a Host string. The validity has been already checked
        //
        // Assuming: UriInfo member is already set at this point
        private unsafe void CreateHostString()
        {
            if (!_syntax.IsSimple)
            {
                lock (_info)
                {
                    // ATTN: Avoid possible recursion through
                    // CreateHostString->Syntax.GetComponents->Uri.GetComponentsHelper->CreateHostString
                    if (NotAny(Flags.ErrorOrParsingRecursion))
                    {
                        _flags |= Flags.ErrorOrParsingRecursion;
                        // Need to get host string through the derived type
                        GetHostViaCustomSyntax();
                        _flags &= ~Flags.ErrorOrParsingRecursion;
                        return;
                    }
                }
            }

            Flags flags = _flags;
            string host = CreateHostStringHelper(_string, _info.Offset.Host, _info.Offset.Path, ref flags, _info);

            // now check on canonical host representation
            if (host.Length != 0)
            {
                // An Authority may need escaping except when it's an inet server address
                if (HostType == Flags.BasicHostType)
                {
                    int idx = 0;
                    Check result;
                    fixed (char* pHost = host)
                    {
                        result = CheckCanonical(pHost, ref idx, host.Length, c_DummyChar);
                    }

                    if ((result & Check.DisplayCanonical) == 0)
                    {
                        // For implicit file the user string must be in perfect display format,
                        // Hence, ignoring complains from CheckCanonical()
                        if (NotAny(Flags.ImplicitFile) || (result & Check.ReservedFound) != 0)
                        {
                            flags |= Flags.HostNotCanonical;
                        }
                    }

                    if (InFact(Flags.ImplicitFile) && (result & (Check.ReservedFound | Check.EscapedCanonical)) != 0)
                    {
                        // need to re-escape this host if any escaped sequence was found
                        result &= ~Check.EscapedCanonical;
                    }

                    if ((result & (Check.EscapedCanonical | Check.BackslashInPath)) != Check.EscapedCanonical)
                    {
                        // we will make a canonical host in _info.Host, but mark that _string holds wrong data
                        flags |= Flags.E_HostNotCanonical;
                        if (NotAny(Flags.UserEscaped))
                        {
                            host = UriHelper.EscapeString(host, checkExistingEscaped: !IsImplicitFile, UriHelper.UnreservedReservedExceptQuestionMarkHash);
                        }
                        else
                        {
                            // We should throw here but currently just accept user input known as invalid
                        }
                    }
                }
                else if (NotAny(Flags.CanonicalDnsHost))
                {
                    // Check to see if we can take the canonical host string out of _string
                    if (_info._moreInfo?.ScopeId is not null)
                    {
                        // IPv6 ScopeId is included when serializing a Uri
                        flags |= (Flags.HostNotCanonical | Flags.E_HostNotCanonical);
                    }
                    else
                    {
                        for (int i = 0; i < host.Length; ++i)
                        {
                            if ((_info.Offset.Host + i) >= _info.Offset.End ||
                                host[i] != _string[_info.Offset.Host + i])
                            {
                                flags |= (Flags.HostNotCanonical | Flags.E_HostNotCanonical);
                                break;
                            }
                        }
                    }
                }
            }

            _info.Host = host;
            InterlockedSetFlags(flags);
        }

        private static string CreateHostStringHelper(string str, int idx, int end, ref Flags flags, UriInfo info)
        {
            bool loopback = false;
            string host;
            switch (flags & Flags.HostTypeMask)
            {
                case Flags.DnsHostType:
                    host = DomainNameHelper.ParseCanonicalName(str, idx, end, ref loopback);
                    break;

                case Flags.IPv6HostType:
                    // The helper will return [...] string that is not suited for Dns.Resolve()
                    host = IPv6AddressHelper.ParseCanonicalName(str.AsSpan(idx), ref loopback, out ReadOnlySpan<char> scopeIdSpan);

                    if (!scopeIdSpan.IsEmpty)
                    {
                        info.MoreInfo.ScopeId = new string(scopeIdSpan);
                    }
                    break;

                case Flags.IPv4HostType:
                    host = IPv4AddressHelper.ParseCanonicalName(str, idx, end, ref loopback);
                    break;

                case Flags.UncHostType:
                    host = UncNameHelper.ParseCanonicalName(str, idx, end, ref loopback);
                    break;

                case Flags.BasicHostType:
                    if (StaticInFact(flags, Flags.DosPath))
                    {
                        host = string.Empty;
                    }
                    else
                    {
                        // This is for a registry-based authority, not relevant for known schemes
                        host = str.Substring(idx, end - idx);
                    }
                    // A empty host would count for a loopback
                    if (host.Length == 0)
                    {
                        loopback = true;
                    }
                    //there will be no port
                    break;

                case Flags.UnknownHostType:
                    //means the host is *not expected* for this uri type
                    host = string.Empty;
                    break;

                default:
                    throw GetException(ParsingError.BadHostName)!;
            }

            if (loopback)
            {
                flags |= Flags.LoopbackHost;
            }
            return host;
        }

        //
        // Called under lock()
        //
        private unsafe void GetHostViaCustomSyntax()
        {
            // A multithreading check
            if (_info.Host != null)
                return;

            string host = _syntax.InternalGetComponents(this, UriComponents.Host, UriFormat.UriEscaped);

            // ATTN: Check on whether recursion has not happened
            if (_info.Host is null)
            {
                ParsingError err = ParsingError.None;
                Flags flags = _flags & ~Flags.HostTypeMask;

                fixed (char* pHost = host)
                {
                    string? newHost = null;
                    if (CheckAuthorityHelper(pHost, 0, host.Length, ref err, ref flags, _syntax, ref newHost) !=
                        host.Length)
                    {
                        // We cannot parse the entire host string
                        flags &= ~Flags.HostTypeMask;
                        flags |= Flags.UnknownHostType;
                    }
                }

                if (err != ParsingError.None || (flags & Flags.HostTypeMask) == Flags.UnknownHostType)
                {
                    // Well, custom parser has returned a not known host type, take it as Basic then.
                    _flags = (_flags & ~Flags.HostTypeMask) | Flags.BasicHostType;
                }
                else
                {
                    host = CreateHostStringHelper(host, 0, host.Length, ref flags, _info);

                    for (int i = 0; i < host.Length; ++i)
                    {
                        if ((_info.Offset.Host + i) >= _info.Offset.End || host[i] != _string[_info.Offset.Host + i])
                        {
                            _flags |= (Flags.HostNotCanonical | Flags.E_HostNotCanonical);
                            break;
                        }
                    }
                    _flags = (_flags & ~Flags.HostTypeMask) | (flags & Flags.HostTypeMask);
                }
            }
            //
            // This is a chance for a custom parser to report a different port value
            //
            string portStr = _syntax.InternalGetComponents(this, UriComponents.StrongPort, UriFormat.UriEscaped);
            int port = 0;
            if (string.IsNullOrEmpty(portStr))
            {
                // It's like no port
                _flags &= ~Flags.NotDefaultPort;
                _flags |= (Flags.PortNotCanonical | Flags.E_PortNotCanonical);
                _info.Offset.PortValue = 0;
            }
            else
            {
                for (int idx = 0; idx < portStr.Length; ++idx)
                {
                    int val = portStr[idx] - '0';
                    if (val < 0 || val > 9 || (port = (port * 10 + val)) > 0xFFFF)
                        throw new UriFormatException(SR.Format(SR.net_uri_PortOutOfRange, _syntax.GetType(), portStr));
                }
                if (port != _info.Offset.PortValue)
                {
                    if (port == _syntax.DefaultPort)
                        _flags &= ~Flags.NotDefaultPort;
                    else
                        _flags |= Flags.NotDefaultPort;

                    _flags |= (Flags.PortNotCanonical | Flags.E_PortNotCanonical);
                    _info.Offset.PortValue = (ushort)port;
                }
            }
            // This must be done as the last thing in this method
            _info.Host = host;
        }

        //
        // An internal shortcut into Uri extensibility API
        //
        internal string GetParts(UriComponents uriParts, UriFormat formatAs)
        {
            return InternalGetComponents(uriParts, formatAs);
        }

        private string GetEscapedParts(UriComponents uriParts)
        {
            Debug.Assert(_info != null && (_flags & Flags.MinimalUriInfoSet) != 0);

            // Which Uri parts are not escaped canonically ?
            // Notice that public UriPart and private Flags must be in Sync so below code can work
            //
            ushort nonCanonical = unchecked((ushort)(((ushort)_flags & ((ushort)Flags.CannotDisplayCanonical << 7)) >> 6));
            if (InFact(Flags.SchemeNotCanonical))
            {
                nonCanonical |= (ushort)Flags.SchemeNotCanonical;
            }

            // We keep separate flags for some of path canonicalization facts
            if ((uriParts & UriComponents.Path) != 0)
            {
                if (InFact(Flags.ShouldBeCompressed | Flags.FirstSlashAbsent | Flags.BackslashInPath))
                {
                    nonCanonical |= (ushort)Flags.PathNotCanonical;
                }
                else if (IsDosPath && _string[_info.Offset.Path + SecuredPathIndex - 1] == '|')
                {
                    // A rare case of c|\
                    nonCanonical |= (ushort)Flags.PathNotCanonical;
                }
            }

            if ((unchecked((ushort)uriParts) & nonCanonical) == 0)
            {
                string? ret = GetUriPartsFromUserString(uriParts);
                if (ret is not null)
                {
                    return ret;
                }
            }

            return RecreateParts(uriParts, nonCanonical, UriFormat.UriEscaped);
        }

        private string GetUnescapedParts(UriComponents uriParts, UriFormat formatAs)
        {
            Debug.Assert(_info != null && (_flags & Flags.MinimalUriInfoSet) != 0);

            // Which Uri parts are not escaped canonically ?
            // Notice that public UriComponents and private Uri.Flags must be in Sync so below code can work
            //
            ushort nonCanonical = unchecked((ushort)((ushort)_flags & (ushort)Flags.CannotDisplayCanonical));

            // We keep separate flags for some of path canonicalization facts
            if ((uriParts & UriComponents.Path) != 0)
            {
                if ((_flags & (Flags.ShouldBeCompressed | Flags.FirstSlashAbsent | Flags.BackslashInPath)) != 0)
                {
                    nonCanonical |= (ushort)Flags.PathNotCanonical;
                }
                else if (IsDosPath && _string[_info.Offset.Path + SecuredPathIndex - 1] == '|')
                {
                    // A rare case of c|\
                    nonCanonical |= (ushort)Flags.PathNotCanonical;
                }
            }

            if ((unchecked((ushort)uriParts) & nonCanonical) == 0)
            {
                string? ret = GetUriPartsFromUserString(uriParts);
                if (ret is not null)
                {
                    return ret;
                }
            }

            return RecreateParts(uriParts, nonCanonical, formatAs);
        }

        private string RecreateParts(UriComponents parts, ushort nonCanonical, UriFormat formatAs)
        {
            EnsureHostString(allowDnsOptimization: false);

            string str = _string;

            var dest = str.Length <= StackallocThreshold
                ? new ValueStringBuilder(stackalloc char[StackallocThreshold])
                : new ValueStringBuilder(str.Length);

            scoped ReadOnlySpan<char> result = RecreateParts(ref dest, str, parts, nonCanonical, formatAs);

            string s = result.ToString();
            dest.Dispose();
            return s;
        }

        private bool TryRecreateParts(scoped Span<char> span, out int charsWritten, UriComponents parts, ushort nonCanonical, UriFormat formatAs)
        {
            EnsureHostString(allowDnsOptimization: false);

            string str = _string;

            var dest = str.Length <= StackallocThreshold
                ? new ValueStringBuilder(stackalloc char[StackallocThreshold])
                : new ValueStringBuilder(str.Length);

            scoped ReadOnlySpan<char> result = RecreateParts(ref dest, str, parts, nonCanonical, formatAs);

            bool copied = result.TryCopyTo(span);
            charsWritten = copied ? result.Length : 0;
            dest.Dispose();
            return copied;
        }

        private ReadOnlySpan<char> RecreateParts(scoped ref ValueStringBuilder dest, string str, UriComponents parts, ushort nonCanonical, UriFormat formatAs)
        {
            Debug.Assert(InFact(Flags.AllUriInfoSet));

            //Scheme and slashes
            if ((parts & UriComponents.Scheme) != 0)
            {
                dest.Append(_syntax.SchemeName);
                if (parts != UriComponents.Scheme)
                {
                    dest.Append(':');
                    if (InFact(Flags.AuthorityFound))
                    {
                        dest.Append('/');
                        dest.Append('/');
                    }
                }
            }

            //UserInfo
            if ((parts & UriComponents.UserInfo) != 0 && InFact(Flags.HasUserInfo))
            {
                ReadOnlySpan<char> slice = str.AsSpan(_info.Offset.User, _info.Offset.Host - _info.Offset.User);

                if ((nonCanonical & (ushort)UriComponents.UserInfo) != 0)
                {
                    switch (formatAs)
                    {
                        case UriFormat.UriEscaped:
                            if (NotAny(Flags.UserEscaped))
                            {
                                UriHelper.EscapeString(slice, ref dest, checkExistingEscaped: true, UriHelper.UnreservedReservedExceptQuestionMarkHash);
                            }
                            else
                            {
                                // We would ideally throw here if InFact(Flags.E_UserNotCanonical) but currently just accept user input known as invalid
                                dest.Append(slice);
                            }
                            break;

                        case UriFormat.SafeUnescaped:
                            UriHelper.UnescapeString(slice[..^1],
                                ref dest, '@', '/', '\\',
                                InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape,
                                _syntax, isQuery: false);
                            dest.Append('@');
                            break;

                        case UriFormat.Unescaped:
                            UriHelper.UnescapeString(slice,
                                ref dest, c_DummyChar, c_DummyChar, c_DummyChar,
                                UnescapeMode.Unescape | UnescapeMode.UnescapeAll,
                                _syntax, isQuery: false);
                            break;

                        default: //V1ToStringUnescape
                            dest.Append(slice);
                            break;
                    }
                }
                else
                {
                    dest.Append(slice);
                }

                if (parts == UriComponents.UserInfo)
                {
                    //strip '@' delimiter
                    dest.Length--;
                }
            }

            // Host
            if ((parts & UriComponents.Host) != 0)
            {
                string host = _info.Host!;

                if (host.Length != 0)
                {
                    UnescapeMode mode;
                    if (formatAs != UriFormat.UriEscaped && HostType == Flags.BasicHostType
                        && (nonCanonical & (ushort)UriComponents.Host) != 0)
                    {
                        // only Basic host could be in the escaped form
                        mode = formatAs == UriFormat.Unescaped
                            ? (UnescapeMode.Unescape | UnescapeMode.UnescapeAll) :
                                (InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape);
                    }
                    else
                    {
                        mode = UnescapeMode.CopyOnly;
                    }

                    var hostBuilder = new ValueStringBuilder(stackalloc char[StackallocThreshold]);

                    // NormalizedHost
                    if ((parts & UriComponents.NormalizedHost) != 0)
                    {
                        host = UriHelper.StripBidiControlCharacters(host, host);

                        // Upconvert any punycode to unicode, xn--pck -> ?
                        if (!DomainNameHelper.TryGetUnicodeEquivalent(host, ref hostBuilder))
                        {
                            hostBuilder.Length = 0;
                        }
                    }

                    UriHelper.UnescapeString(hostBuilder.Length == 0 ? host : hostBuilder.AsSpan(),
                        ref dest, '/', '?', '#',
                        mode,
                        _syntax, isQuery: false);

                    hostBuilder.Dispose();

                    // A fix up only for SerializationInfo and IpV6 host with a scopeID
                    if ((parts & UriComponents.SerializationInfoString) != 0 && HostType == Flags.IPv6HostType && _info._moreInfo?.ScopeId is string scopeId)
                    {
                        dest.Length--;
                        dest.Append(scopeId);
                        dest.Append(']');
                    }
                }
            }

            //Port (always wants a ':' delimiter if got to this method)
            if ((parts & UriComponents.Port) != 0 &&
                (InFact(Flags.NotDefaultPort) || ((parts & UriComponents.StrongPort) != 0 && _syntax.DefaultPort != UriParser.NoDefaultPort)))
            {
                dest.Append(':');

                const int MaxUshortLength = 5;
                bool success = _info.Offset.PortValue.TryFormat(dest.AppendSpan(MaxUshortLength), out int charsWritten);
                Debug.Assert(success);
                dest.Length -= MaxUshortLength - charsWritten;
            }

            //Path
            if ((parts & UriComponents.Path) != 0)
            {
                GetCanonicalPath(ref dest, formatAs);

                // (possibly strip the leading '/' delimiter)
                if (parts == UriComponents.Path)
                {
                    int offset;
                    if (InFact(Flags.AuthorityFound) && dest.Length != 0 && dest[0] == '/')
                    {
                        offset = 1;
                    }
                    else
                    {
                        offset = 0;
                    }

                    return dest.AsSpan(offset);
                }
            }

            //Query (possibly strip the '?' delimiter)
            if ((parts & UriComponents.Query) != 0 && _info.Offset.Query < _info.Offset.Fragment)
            {
                int offset = (_info.Offset.Query + 1);
                if (parts != UriComponents.Query)
                    dest.Append('?');

                UnescapeMode mode = UnescapeMode.CopyOnly;

                if ((nonCanonical & (ushort)UriComponents.Query) != 0)
                {
                    if (formatAs == UriFormat.UriEscaped)
                    {
                        if (NotAny(Flags.UserEscaped))
                        {
                            UriHelper.EscapeString(
                                str.AsSpan(offset, _info.Offset.Fragment - offset),
                                ref dest, checkExistingEscaped: true, UriHelper.UnreservedReservedExceptHash);

                            goto AfterQuery;
                        }
                    }
                    else
                    {
                        mode = formatAs switch
                        {
                            V1ToStringUnescape => (InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape) | UnescapeMode.V1ToStringFlag,
                            UriFormat.Unescaped => UnescapeMode.Unescape | UnescapeMode.UnescapeAll,
                            _ => InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape
                        };
                    }
                }

                UriHelper.UnescapeString(str, offset, _info.Offset.Fragment,
                    ref dest, '#', c_DummyChar, c_DummyChar,
                    mode, _syntax, isQuery: true);
            }
        AfterQuery:

            //Fragment (possibly strip the '#' delimiter)
            if ((parts & UriComponents.Fragment) != 0 && _info.Offset.Fragment < _info.Offset.End)
            {
                int offset = _info.Offset.Fragment + 1;
                if (parts != UriComponents.Fragment)
                    dest.Append('#');

                UnescapeMode mode = UnescapeMode.CopyOnly;

                if ((nonCanonical & (ushort)UriComponents.Fragment) != 0)
                {
                    if (formatAs == UriFormat.UriEscaped)
                    {
                        if (NotAny(Flags.UserEscaped))
                        {
                            UriHelper.EscapeString(
                                str.AsSpan(offset, _info.Offset.End - offset),
                                ref dest, checkExistingEscaped: true, UriHelper.UnreservedReserved);

                            goto AfterFragment;
                        }
                    }
                    else
                    {
                        mode = formatAs switch
                        {
                            V1ToStringUnescape => (InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape) | UnescapeMode.V1ToStringFlag,
                            UriFormat.Unescaped => UnescapeMode.Unescape | UnescapeMode.UnescapeAll,
                            _ => InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape
                        };
                    }
                }

                UriHelper.UnescapeString(str, offset, _info.Offset.End,
                    ref dest, '#', c_DummyChar, c_DummyChar,
                    mode, _syntax, isQuery: false);
            }

        AfterFragment:
            return dest.AsSpan();
        }

        //
        // This method is called only if the user string has a canonical representation
        // of requested parts
        //
        private string? GetUriPartsFromUserString(UriComponents uriParts)
        {
            int delimiterAwareIdx;

            switch (uriParts & ~UriComponents.KeepDelimiter)
            {
                // For FindServicePoint perf
                case UriComponents.Scheme | UriComponents.Host | UriComponents.Port:
                    if (!InFact(Flags.HasUserInfo))
                        return _string.Substring(_info.Offset.Scheme, _info.Offset.Path - _info.Offset.Scheme);

                    return string.Concat(
                        _string.AsSpan(_info.Offset.Scheme, _info.Offset.User - _info.Offset.Scheme),
                        _string.AsSpan(_info.Offset.Host, _info.Offset.Path - _info.Offset.Host));

                // For HttpWebRequest.ConnectHostAndPort perf
                case UriComponents.HostAndPort:  //Host|StrongPort

                    if (!InFact(Flags.HasUserInfo))
                        goto case UriComponents.StrongAuthority;

                    if (InFact(Flags.NotDefaultPort) || _syntax.DefaultPort == UriParser.NoDefaultPort)
                        return _string.Substring(_info.Offset.Host, _info.Offset.Path - _info.Offset.Host);

                    return string.Concat(_string.AsSpan(_info.Offset.Host, _info.Offset.Path - _info.Offset.Host),
                        ":", _info.Offset.PortValue.ToString(CultureInfo.InvariantCulture));

                // For an obvious common case perf
                case UriComponents.AbsoluteUri:     //Scheme|UserInfo|Host|Port|Path|Query|Fragment,
                    if (_info.Offset.Scheme == 0 && _info.Offset.End == _string.Length)
                        return _string;

                    return _string.Substring(_info.Offset.Scheme, _info.Offset.End - _info.Offset.Scheme);

                // For Uri.Equals() and HttpWebRequest through a proxy perf
                case UriComponents.HttpRequestUrl:   //Scheme|Host|Port|Path|Query,
                    if (InFact(Flags.HasUserInfo))
                    {
                        return string.Concat(
                            _string.AsSpan(_info.Offset.Scheme, _info.Offset.User - _info.Offset.Scheme),
                            _string.AsSpan(_info.Offset.Host, _info.Offset.Fragment - _info.Offset.Host));
                    }
                    if (_info.Offset.Scheme == 0 && _info.Offset.Fragment == _string.Length)
                        return _string;

                    return _string.Substring(_info.Offset.Scheme, _info.Offset.Fragment - _info.Offset.Scheme);

                // For CombineUri() perf
                case UriComponents.SchemeAndServer | UriComponents.UserInfo:
                    return _string.Substring(_info.Offset.Scheme, _info.Offset.Path - _info.Offset.Scheme);

                // For Cache perf
                case (UriComponents.AbsoluteUri & ~UriComponents.Fragment):
                    if (_info.Offset.Scheme == 0 && _info.Offset.Fragment == _string.Length)
                        return _string;

                    return _string.Substring(_info.Offset.Scheme, _info.Offset.Fragment - _info.Offset.Scheme);


                // Strip scheme delimiter if was not requested
                case UriComponents.Scheme:
                    if (uriParts != UriComponents.Scheme)
                        return _string.Substring(_info.Offset.Scheme, _info.Offset.User - _info.Offset.Scheme);

                    return _syntax.SchemeName;

                // KeepDelimiter makes no sense for this component
                case UriComponents.Host:
                    int idx = _info.Offset.Path;
                    if (InFact(Flags.NotDefaultPort | Flags.PortNotCanonical))
                    {
                        //Means we do have ':' after the host
                        while (_string[--idx] != ':')
                            ;
                    }
                    return (idx - _info.Offset.Host == 0) ? string.Empty : _string.Substring(_info.Offset.Host,
                        idx - _info.Offset.Host);

                case UriComponents.Path:

                    // Strip the leading '/' for a hierarchical URI if no delimiter was requested
                    if (uriParts == UriComponents.Path && InFact(Flags.AuthorityFound) &&
                        _info.Offset.End > _info.Offset.Path && _string[_info.Offset.Path] == '/')
                        delimiterAwareIdx = _info.Offset.Path + 1;
                    else
                        delimiterAwareIdx = _info.Offset.Path;

                    if (delimiterAwareIdx >= _info.Offset.Query)
                        return string.Empty;


                    return _string.Substring(delimiterAwareIdx, _info.Offset.Query - delimiterAwareIdx);

                case UriComponents.Query:
                    // Strip the '?' if no delimiter was requested
                    if (uriParts == UriComponents.Query)
                        delimiterAwareIdx = _info.Offset.Query + 1;
                    else
                        delimiterAwareIdx = _info.Offset.Query;

                    if (delimiterAwareIdx >= _info.Offset.Fragment)
                        return string.Empty;

                    return _string.Substring(delimiterAwareIdx, _info.Offset.Fragment - delimiterAwareIdx);

                case UriComponents.Fragment:
                    // Strip the '#' if no delimiter was requested
                    if (uriParts == UriComponents.Fragment)
                        delimiterAwareIdx = _info.Offset.Fragment + 1;
                    else
                        delimiterAwareIdx = _info.Offset.Fragment;

                    if (delimiterAwareIdx >= _info.Offset.End)
                        return string.Empty;

                    return _string.Substring(delimiterAwareIdx, _info.Offset.End - delimiterAwareIdx);

                case UriComponents.UserInfo | UriComponents.Host | UriComponents.Port:
                    return (_info.Offset.Path - _info.Offset.User == 0) ? string.Empty :
                        _string.Substring(_info.Offset.User, _info.Offset.Path - _info.Offset.User);

                case UriComponents.StrongAuthority:  //UserInfo|Host|StrongPort
                    if (InFact(Flags.NotDefaultPort) || _syntax.DefaultPort == UriParser.NoDefaultPort)
                        goto case UriComponents.UserInfo | UriComponents.Host | UriComponents.Port;

                    return string.Concat(_string.AsSpan(_info.Offset.User, _info.Offset.Path - _info.Offset.User),
                        ":", _info.Offset.PortValue.ToString(CultureInfo.InvariantCulture));

                case UriComponents.PathAndQuery:        //Path|Query,
                    return _string.Substring(_info.Offset.Path, _info.Offset.Fragment - _info.Offset.Path);

                case UriComponents.HttpRequestUrl | UriComponents.Fragment: //Scheme|Host|Port|Path|Query|Fragment,
                    if (InFact(Flags.HasUserInfo))
                    {
                        return string.Concat(
                            _string.AsSpan(_info.Offset.Scheme, _info.Offset.User - _info.Offset.Scheme),
                            _string.AsSpan(_info.Offset.Host, _info.Offset.End - _info.Offset.Host));
                    }
                    if (_info.Offset.Scheme == 0 && _info.Offset.End == _string.Length)
                        return _string;

                    return _string.Substring(_info.Offset.Scheme, _info.Offset.End - _info.Offset.Scheme);

                case UriComponents.PathAndQuery | UriComponents.Fragment:  //LocalUrl|Fragment
                    return _string.Substring(_info.Offset.Path, _info.Offset.End - _info.Offset.Path);

                case UriComponents.UserInfo:
                    // Strip the '@' if no delimiter was requested

                    if (NotAny(Flags.HasUserInfo))
                        return string.Empty;

                    if (uriParts == UriComponents.UserInfo)
                        delimiterAwareIdx = _info.Offset.Host - 1;
                    else
                        delimiterAwareIdx = _info.Offset.Host;

                    if (_info.Offset.User >= delimiterAwareIdx)
                        return string.Empty;

                    return _string.Substring(_info.Offset.User, delimiterAwareIdx - _info.Offset.User);

                default:
                    return null;
            }
        }

        // Cut trailing spaces
        private static void GetLengthWithoutTrailingSpaces(string str, ref int length, int idx)
        {
            // to avoid dereferencing ref length parameter for every update
            int local = length;
            while (local > idx && UriHelper.IsLWS(str[local - 1])) --local;
            length = local;
        }

        //
        //This method does:
        //  - Creates _info member
        //  - checks all components up to path on their canonical representation
        //  - continues parsing starting the path position
        //  - Sets the offsets of remaining components
        //  - Sets the Canonicalization flags if applied
        //  - Will NOT create MoreInfo members
        //
        private unsafe void ParseRemaining()
        {
            // ensure we parsed up to the path
            EnsureUriInfo();

            Flags cF = Flags.Zero;

            if (UserDrivenParsing)
                goto Done;

            // Do we have to continue building Iri'zed string from original string
            bool buildIriStringFromPath = InFact(Flags.HasUnicode);

            int origIdx; // stores index to switched original string
            int idx = _info.Offset.Scheme;
            int length = _string.Length;
            Check result = Check.None;
            UriSyntaxFlags syntaxFlags = _syntax.Flags;

            // _info.Offset values may be parsed twice but we lock only on _flags update.

            fixed (char* str = _string)
            {
                GetLengthWithoutTrailingSpaces(_string, ref length, idx);

                if (IsImplicitFile)
                {
                    cF |= Flags.SchemeNotCanonical;
                }
                else
                {
                    int i;
                    string schemeName = _syntax.SchemeName;
                    for (i = 0; i < schemeName.Length; ++i)
                    {
                        if (schemeName[i] != str[idx + i])
                            cF |= Flags.SchemeNotCanonical;
                    }
                    // For an authority Uri only // after the scheme would be canonical
                    // (for compatibility with: http:\\host)
                    if (((_flags & Flags.AuthorityFound) != 0) && (idx + i + 3 >= length || str[idx + i + 1] != '/' ||
                        str[idx + i + 2] != '/'))
                    {
                        cF |= Flags.SchemeNotCanonical;
                    }
                }


                //Check the form of the user info
                if ((_flags & Flags.HasUserInfo) != 0)
                {
                    idx = _info.Offset.User;
                    result = CheckCanonical(str, ref idx, _info.Offset.Host, '@');
                    if ((result & Check.DisplayCanonical) == 0)
                    {
                        cF |= Flags.UserNotCanonical;
                    }
                    if ((result & (Check.EscapedCanonical | Check.BackslashInPath)) != Check.EscapedCanonical)
                    {
                        cF |= Flags.E_UserNotCanonical;
                    }
                    if (IriParsing && ((result & (Check.DisplayCanonical | Check.EscapedCanonical | Check.BackslashInPath
                                                    | Check.FoundNonAscii | Check.NotIriCanonical))
                                                    == (Check.DisplayCanonical | Check.FoundNonAscii)))
                    {
                        cF |= Flags.UserIriCanonical;
                    }
                }
            }
            //
            // Delay canonical Host checking to avoid creation of a host string
            // Will do that on demand.
            //


            //
            //We have already checked on the port in EnsureUriInfo() that calls CreateUriInfo
            //

            //
            // Parsing the Path if any
            //

            // For iri parsing if we found unicode the idx has offset into _originalUnicodeString..
            // so restart parsing from there and make _info.Offset.Path as _string.Length

            idx = _info.Offset.Path;
            origIdx = _info.Offset.Path;

            if (buildIriStringFromPath)
            {
                DebugAssertInCtor();

                // Dos/Unix paths have no host.  Other schemes cleared/set _string with host information in PrivateParseMinimal.
                if (IsFile && !IsUncPath)
                {
                    if (IsImplicitFile)
                    {
                        _string = string.Empty;
                    }
                    else
                    {
                        _string = _syntax.SchemeName + SchemeDelimiter;
                    }

                    _info.Offset.Scheme = 0;
                    _info.Offset.User = _string.Length;
                    _info.Offset.Host = _string.Length;
                }

                _info.Offset.Path = _string.Length;
                idx = _info.Offset.Path;
            }

            // If the user explicitly disabled canonicalization, only figure out the offsets
            if (DisablePathAndQueryCanonicalization)
            {
                if (buildIriStringFromPath)
                {
                    DebugAssertInCtor();
                    _string = string.Concat(_string, _originalUnicodeString.AsSpan(origIdx));
                }

                string str = _string;

                if (IsImplicitFile || (syntaxFlags & UriSyntaxFlags.MayHaveQuery) == 0)
                {
                    idx = str.Length;
                }
                else
                {
                    idx = str.IndexOf('?');
                    if (idx == -1)
                    {
                        idx = str.Length;
                    }
                }

                _info.Offset.Query = idx;
                _info.Offset.Fragment = str.Length; // There is no fragment in DisablePathAndQueryCanonicalization mode
                _info.Offset.End = str.Length;

                goto Done;
            }

            //Some uris do not have a query
            //    When '?' is passed as delimiter, then it's special case
            //    so both '?' and '#' will work as delimiters
            if (buildIriStringFromPath)
            {
                DebugAssertInCtor();

                int offset = origIdx;
                if (IsImplicitFile || ((syntaxFlags & (UriSyntaxFlags.MayHaveQuery | UriSyntaxFlags.MayHaveFragment)) == 0))
                {
                    origIdx = _originalUnicodeString.Length;
                }
                else
                {
                    ReadOnlySpan<char> span = _originalUnicodeString.AsSpan(origIdx);
                    int index;
                    if (_syntax.InFact(UriSyntaxFlags.MayHaveQuery))
                    {
                        if (_syntax.InFact(UriSyntaxFlags.MayHaveFragment))
                        {
                            index = span.IndexOfAny('?', '#');
                        }
                        else
                        {
                            index = span.IndexOf('?');
                        }
                    }
                    else
                    {
                        Debug.Assert(_syntax.InFact(UriSyntaxFlags.MayHaveFragment));
                        index = span.IndexOf('#');
                    }
                    origIdx = index == -1 ? _originalUnicodeString.Length : (index + origIdx);
                }

                _string += EscapeUnescapeIri(_originalUnicodeString, offset, origIdx, UriComponents.Path);

                length = _string.Length;
                // We need to be sure that there isn't a '?' separated from the path by spaces.
                if (_string == _originalUnicodeString)
                {
                    GetLengthWithoutTrailingSpaces(_string, ref length, idx);
                }
            }

            fixed (char* str = _string)
            {
                if (IsImplicitFile || ((syntaxFlags & (UriSyntaxFlags.MayHaveQuery | UriSyntaxFlags.MayHaveFragment)) == 0))
                {
                    result = CheckCanonical(str, ref idx, length, c_DummyChar);
                }
                else
                {
                    result = CheckCanonical(str, ref idx, length, (((syntaxFlags & UriSyntaxFlags.MayHaveQuery) != 0)
                        ? '?' : _syntax.InFact(UriSyntaxFlags.MayHaveFragment) ? '#' : c_EOL));
                }

                // ATTN:
                // This may render problems for unknown schemes, but in general for an authority based Uri
                // (that has slashes) a path should start with "/"
                // This becomes more interesting knowing how a file uri is used in "file://c:/path"
                // It will be converted to file:///c:/path
                //
                // However, even more interesting is that vsmacros://c:\path will not add the third slash in the _canoical_ case
                //
                // We use special syntax flag to check if the path is rooted, i.e. has a first slash
                //
                if (((_flags & Flags.AuthorityFound) != 0) && ((syntaxFlags & UriSyntaxFlags.PathIsRooted) != 0)
                    && (_info.Offset.Path == length || (str[_info.Offset.Path] != '/' && str[_info.Offset.Path] != '\\')))
                {
                    cF |= Flags.FirstSlashAbsent;
                }
            }
            // Check the need for compression or backslashes conversion
            // we included IsDosPath since it may come with other than FILE uri, for ex. scheme://C:\path
            // (This is very unfortunate that the original design has included that feature)
            bool nonCanonical = false;
            if (IsDosPath || (((_flags & Flags.AuthorityFound) != 0) &&
                (((syntaxFlags & (UriSyntaxFlags.CompressPath | UriSyntaxFlags.ConvertPathSlashes)) != 0) ||
                _syntax.InFact(UriSyntaxFlags.UnEscapeDotsAndSlashes))))
            {
                if (((result & Check.DotSlashEscaped) != 0) && _syntax.InFact(UriSyntaxFlags.UnEscapeDotsAndSlashes))
                {
                    cF |= (Flags.E_PathNotCanonical | Flags.PathNotCanonical);
                    nonCanonical = true;
                }

                if (((syntaxFlags & (UriSyntaxFlags.ConvertPathSlashes)) != 0) && (result & Check.BackslashInPath) != 0)
                {
                    cF |= (Flags.E_PathNotCanonical | Flags.PathNotCanonical);
                    nonCanonical = true;
                }

                if (((syntaxFlags & (UriSyntaxFlags.CompressPath)) != 0) && ((cF & Flags.E_PathNotCanonical) != 0 ||
                    (result & Check.DotSlashAttn) != 0))
                {
                    cF |= Flags.ShouldBeCompressed;
                }

                if ((result & Check.BackslashInPath) != 0)
                    cF |= Flags.BackslashInPath;
            }
            else if ((result & Check.BackslashInPath) != 0)
            {
                // for a "generic" path '\' should be escaped
                cF |= Flags.E_PathNotCanonical;
                nonCanonical = true;
            }

            if ((result & Check.DisplayCanonical) == 0)
            {
                // For implicit file the user string is usually in perfect display format,
                // Hence, ignoring complains from CheckCanonical()
                // V1 compat. In fact we should simply ignore dontEscape parameter for Implicit file.
                // Currently we don't.
                if (((_flags & Flags.ImplicitFile) == 0) || ((_flags & Flags.UserEscaped) != 0) ||
                    (result & Check.ReservedFound) != 0)
                {
                    //means it's found as escaped or has unescaped Reserved Characters
                    cF |= Flags.PathNotCanonical;
                    nonCanonical = true;
                }
            }

            if (((_flags & Flags.ImplicitFile) != 0) && (result & (Check.ReservedFound | Check.EscapedCanonical)) != 0)
            {
                // need to escape reserved chars or re-escape '%' if an "escaped sequence" was found
                result &= ~Check.EscapedCanonical;
            }

            if ((result & Check.EscapedCanonical) == 0)
            {
                //means it's found as not completely escaped
                cF |= Flags.E_PathNotCanonical;
            }

            if (IriParsing && !nonCanonical && ((result & (Check.DisplayCanonical | Check.EscapedCanonical
                            | Check.FoundNonAscii | Check.NotIriCanonical))
                            == (Check.DisplayCanonical | Check.FoundNonAscii)))
            {
                cF |= Flags.PathIriCanonical;
            }

            //
            //Now we've got to parse the Query if any. Note that Query requires the presence of '?'
            //
            if (buildIriStringFromPath)
            {
                DebugAssertInCtor();

                int offset = origIdx;

                if (origIdx < _originalUnicodeString.Length && _originalUnicodeString[origIdx] == '?')
                {
                    if ((syntaxFlags & (UriSyntaxFlags.MayHaveFragment)) != 0)
                    {
                        ++origIdx; // This is to exclude first '?' character from checking
                        int index = _originalUnicodeString.AsSpan(origIdx).IndexOf('#');
                        origIdx = index == -1 ? _originalUnicodeString.Length : (index + origIdx);
                    }
                    else
                    {
                        origIdx = _originalUnicodeString.Length;
                    }

                    _string += EscapeUnescapeIri(_originalUnicodeString, offset, origIdx, UriComponents.Query);

                    length = _string.Length;
                    // We need to be sure that there isn't a '#' separated from the query by spaces.
                    if (_string == _originalUnicodeString)
                    {
                        GetLengthWithoutTrailingSpaces(_string, ref length, idx);
                    }
                }
            }

            _info.Offset.Query = idx;

            fixed (char* str = _string)
            {
                if (idx < length && str[idx] == '?')
                {
                    ++idx; // This is to exclude first '?' character from checking
                    result = CheckCanonical(str, ref idx, length, ((syntaxFlags & (UriSyntaxFlags.MayHaveFragment)) != 0)
                        ? '#' : c_EOL);
                    if ((result & Check.DisplayCanonical) == 0)
                    {
                        cF |= Flags.QueryNotCanonical;
                    }

                    if ((result & (Check.EscapedCanonical | Check.BackslashInPath)) != Check.EscapedCanonical)
                    {
                        cF |= Flags.E_QueryNotCanonical;
                    }

                    if (IriParsing && ((result & (Check.DisplayCanonical | Check.EscapedCanonical | Check.BackslashInPath
                                | Check.FoundNonAscii | Check.NotIriCanonical))
                                == (Check.DisplayCanonical | Check.FoundNonAscii)))
                    {
                        cF |= Flags.QueryIriCanonical;
                    }
                }
            }
            //
            //Now we've got to parse the Fragment if any. Note that Fragment requires the presence of '#'
            //
            if (buildIriStringFromPath)
            {
                DebugAssertInCtor();

                int offset = origIdx;

                if (origIdx < _originalUnicodeString.Length && _originalUnicodeString[origIdx] == '#')
                {
                    origIdx = _originalUnicodeString.Length;

                    _string += EscapeUnescapeIri(_originalUnicodeString, offset, origIdx, UriComponents.Fragment);

                    length = _string.Length;
                    // we don't need to check _originalUnicodeString == _string because # is last part
                    GetLengthWithoutTrailingSpaces(_string, ref length, idx);
                }
            }

            _info.Offset.Fragment = idx;

            fixed (char* str = _string)
            {
                if (idx < length && str[idx] == '#')
                {
                    ++idx; // This is to exclude first '#' character from checking
                    //We don't using c_DummyChar since want to allow '?' and '#' as unescaped
                    result = CheckCanonical(str, ref idx, length, c_EOL);
                    if ((result & Check.DisplayCanonical) == 0)
                    {
                        cF |= Flags.FragmentNotCanonical;
                    }

                    if ((result & (Check.EscapedCanonical | Check.BackslashInPath)) != Check.EscapedCanonical)
                    {
                        cF |= Flags.E_FragmentNotCanonical;
                    }

                    if (IriParsing && ((result & (Check.DisplayCanonical | Check.EscapedCanonical | Check.BackslashInPath
                                | Check.FoundNonAscii | Check.NotIriCanonical))
                                == (Check.DisplayCanonical | Check.FoundNonAscii)))
                    {
                        cF |= Flags.FragmentIriCanonical;
                    }
                }
            }
            _info.Offset.End = idx;

        Done:
            cF |= Flags.AllUriInfoSet;
            InterlockedSetFlags(cF);
        }

        // verifies the syntax of the scheme part
        // Checks on implicit File: scheme due to simple Dos/Unc path passed
        // returns the start of the next component  position
        private static int ParseSchemeCheckImplicitFile(string uriString, ref ParsingError err, ref Flags flags, ref UriParser? syntax)
        {
            Debug.Assert(err == ParsingError.None);
            Debug.Assert((flags & Flags.Debug_LeftConstructor) == 0);

            int i = 0;

            // skip whitespace
            while ((uint)i < (uint)uriString.Length && UriHelper.IsLWS(uriString[i]))
            {
                i++;
            }

            // Unix: Unix path?
            // A path starting with 2 / or \ (including mixed) is treated as UNC and will be matched below
            if (!OperatingSystem.IsWindows() &&
                (uint)i < (uint)uriString.Length && uriString[i] == '/' &&
                ((uint)(i + 1) >= (uint)uriString.Length || uriString[i + 1] is not ('/' or '\\')))
            {
                flags |= (Flags.UnixPath | Flags.ImplicitFile | Flags.AuthorityFound);
                syntax = UriParser.UnixFileUri;
                return i;
            }

            // Find the colon.
            // Note that we don't support one-letter schemes that will be put into a DOS path bucket
            int colonOffset = uriString.AsSpan(i).IndexOf(':');

            // NB: A string must have at least 3 characters and at least 1 before ':'
            if ((uint)(i + 2) >= (uint)uriString.Length ||
                colonOffset == 0 ||
                // Redundant checks to eliminate range checks below
                (uint)i >= (uint)uriString.Length ||
                (uint)(i + 1) >= (uint)uriString.Length)
            {
                err = ParsingError.BadFormat;
                return 0;
            }

            // Check for supported special cases like a DOS file path OR a UNC share path
            // NB: A string may not have ':' if this is a UNC path
            if (uriString[i + 1] is ':' or '|')
            {
                // DOS-like path?
                if (char.IsAsciiLetter(uriString[i]))
                {
                    if (uriString[i + 2] is '\\' or '/')
                    {
                        flags |= (Flags.DosPath | Flags.ImplicitFile | Flags.AuthorityFound);
                        syntax = UriParser.FileUri;
                        return i;
                    }

                    err = ParsingError.MustRootedPath;
                    return 0;
                }

                err = uriString[i + 1] == ':' ? ParsingError.BadScheme : ParsingError.BadFormat;
                return 0;
            }
            else if (uriString[i] is '/' or '\\')
            {
                // UNC share?
                if (uriString[i + 1] is '\\' or '/')
                {
                    flags |= (Flags.UncPath | Flags.ImplicitFile | Flags.AuthorityFound);
                    syntax = UriParser.FileUri;
                    i += 2;

                    // V1.1 compat this will simply eat any slashes prepended to a UNC path
                    while ((uint)i < (uint)uriString.Length && uriString[i] is '/' or '\\')
                    {
                        i++;
                    }

                    return i;
                }

                err = ParsingError.BadFormat;
                return 0;
            }

            if (colonOffset < 0)
            {
                err = ParsingError.BadFormat;
                return 0;
            }

            // This is a potentially valid scheme, but we have not identified it yet.
            // Check for illegal characters, canonicalize, and check the length.
            syntax = CheckSchemeSyntax(uriString.AsSpan(i, colonOffset), ref err);
            if (syntax is null)
            {
                return 0;
            }
            return i + colonOffset + 1;
        }

        // This will check whether a scheme string follows the rules
        private static UriParser? CheckSchemeSyntax(ReadOnlySpan<char> scheme, ref ParsingError error)
        {
            Debug.Assert(error == ParsingError.None);

            switch (scheme.Length)
            {
                case 2:
                    if (scheme.Equals("ws", StringComparison.OrdinalIgnoreCase)) return UriParser.WsUri;
                    break;

                case 3:
                    if (scheme.Equals("wss", StringComparison.OrdinalIgnoreCase)) return UriParser.WssUri;
                    if (scheme.Equals("ftp", StringComparison.OrdinalIgnoreCase)) return UriParser.FtpUri;
                    break;

                case 4:
                    if (scheme.Equals("http", StringComparison.OrdinalIgnoreCase)) return UriParser.HttpUri;
                    if (scheme.Equals("file", StringComparison.OrdinalIgnoreCase)) return UriParser.FileUri;
                    if (scheme.Equals("uuid", StringComparison.OrdinalIgnoreCase)) return UriParser.UuidUri;
                    if (scheme.Equals("nntp", StringComparison.OrdinalIgnoreCase)) return UriParser.NntpUri;
                    if (scheme.Equals("ldap", StringComparison.OrdinalIgnoreCase)) return UriParser.LdapUri;
                    if (scheme.Equals("news", StringComparison.OrdinalIgnoreCase)) return UriParser.NewsUri;
                    break;

                case 5:
                    if (scheme.Equals("https", StringComparison.OrdinalIgnoreCase)) return UriParser.HttpsUri;
                    break;

                case 6:
                    if (scheme.Equals("mailto", StringComparison.OrdinalIgnoreCase)) return UriParser.MailToUri;
                    if (scheme.Equals("gopher", StringComparison.OrdinalIgnoreCase)) return UriParser.GopherUri;
                    if (scheme.Equals("telnet", StringComparison.OrdinalIgnoreCase)) return UriParser.TelnetUri;
                    break;

                case 7:
                    if (scheme.Equals("net.tcp", StringComparison.OrdinalIgnoreCase)) return UriParser.NetTcpUri;
                    break;

                case 8:
                    if (scheme.Equals("net.pipe", StringComparison.OrdinalIgnoreCase)) return UriParser.NetPipeUri;
                    break;
            }

            // scheme = alpha *(alpha | digit | '+' | '-' | '.')
            if (scheme.Length == 0 ||
                !char.IsAsciiLetter(scheme[0]) ||
                scheme.ContainsAnyExcept(s_schemeChars))
            {
                error = ParsingError.BadScheme;
                return null;
            }

            if (scheme.Length > SchemeLengthLimit)
            {
                error = ParsingError.SchemeLimit;
                return null;
            }

            // Then look up the syntax in a string-based table.
            return UriParser.FindOrFetchAsUnknownV1Syntax(UriHelper.SpanToLowerInvariantString(scheme));
        }

        //
        // Checks the syntax of an authority component. It may also get a userInfo if present
        // Returns an error if no/mailformed authority found
        // Does not NOT touch _info
        // Returns position of the Path component
        //
        // Must be called in the ctor only
        private unsafe int CheckAuthorityHelper(char* pString, int idx, int length,
            ref ParsingError err, ref Flags flags, UriParser syntax, ref string? newHost)
        {
            Debug.Assert((_flags & Flags.Debug_LeftConstructor) == 0 || (!_syntax.IsSimple && Monitor.IsEntered(_info)));

            int end = length;
            char ch;
            int startInput = idx;
            int start = idx;
            newHost = null;
            bool hasUnicode = ((flags & Flags.HasUnicode) != 0);
            UriSyntaxFlags syntaxFlags = syntax.Flags;

            Debug.Assert((_flags & Flags.HasUserInfo) == 0 && (_flags & Flags.HostTypeMask) == 0);

            // need to build new Iri'zed string
            if (hasUnicode)
            {
                newHost = _originalUnicodeString.Substring(0, startInput);
            }

            //Special case is an empty authority
            if (idx == length || ((ch = pString[idx]) == '/' || (ch == '\\' && StaticIsFile(syntax)) || ch == '#' || ch == '?'))
            {
                if (syntax.InFact(UriSyntaxFlags.AllowEmptyHost))
                {
                    flags &= ~Flags.UncPath;    //UNC cannot have an empty hostname
                    if (StaticInFact(flags, Flags.ImplicitFile))
                        err = ParsingError.BadHostName;
                    else
                        flags |= Flags.BasicHostType;
                }
                else
                    err = ParsingError.BadHostName;

                return idx;
            }

            // Attempt to parse user info first

            if ((syntaxFlags & UriSyntaxFlags.MayHaveUserInfo) != 0)
            {
                for (; start < end; ++start)
                {
                    if (start == end - 1 || pString[start] == '?' || pString[start] == '#' || pString[start] == '\\' ||
                        pString[start] == '/')
                    {
                        start = idx;
                        break;
                    }
                    else if (pString[start] == '@')
                    {
                        flags |= Flags.HasUserInfo;

                        // Iri'ze userinfo
                        if (hasUnicode)
                        {
                            // Normalize user info
                            newHost += IriHelper.EscapeUnescapeIri(pString, startInput, start + 1, UriComponents.UserInfo);
                        }
                        ++start;
                        ch = pString[start];
                        break;
                    }
                }
            }

            if (ch == '[' && syntax.InFact(UriSyntaxFlags.AllowIPv6Host) &&
                IPv6AddressHelper.IsValid(pString, start + 1, ref end))
            {
                flags |= Flags.IPv6HostType;

                if (hasUnicode)
                {
                    newHost = string.Concat(newHost, new ReadOnlySpan<char>(pString + start, end - start));
                }
            }
            else if (char.IsAsciiDigit(ch) && syntax.InFact(UriSyntaxFlags.AllowIPv4Host) &&
                IPv4AddressHelper.IsValid(pString, start, ref end, false, StaticNotAny(flags, Flags.ImplicitFile), syntax.InFact(UriSyntaxFlags.V1_UnknownUri)))
            {
                flags |= Flags.IPv4HostType;

                if (hasUnicode)
                {
                    newHost = string.Concat(newHost, new ReadOnlySpan<char>(pString + start, end - start));
                }
            }
            else if (((syntaxFlags & UriSyntaxFlags.AllowDnsHost) != 0) && !IriParsingStatic(syntax) &&
                DomainNameHelper.IsValid(new ReadOnlySpan<char>(pString + start, end - start), iri: false, StaticNotAny(flags, Flags.ImplicitFile), out int domainNameLength))
            {
                Debug.Assert(!hasUnicode);

                end = start + domainNameLength;

                // comes here if there are only ascii chars in host with original parsing and no Iri
                flags |= Flags.DnsHostType;

                // Canonical DNS hostnames don't contain uppercase letters
                if (!new ReadOnlySpan<char>(pString + start, domainNameLength).ContainsAnyInRange('A', 'Z'))
                {
                    flags |= Flags.CanonicalDnsHost;
                }
            }
            else if (((syntaxFlags & UriSyntaxFlags.AllowDnsHost) != 0) &&
                (hasUnicode || syntax.InFact(UriSyntaxFlags.AllowIdn)) &&
                DomainNameHelper.IsValid(new ReadOnlySpan<char>(pString + start, end - start), iri: true, StaticNotAny(flags, Flags.ImplicitFile), out domainNameLength))
            {
                end = start + domainNameLength;

                CheckAuthorityHelperHandleDnsIri(pString, start, end, hasUnicode,
                    ref flags, ref newHost, ref err);
            }
            else if ((syntaxFlags & UriSyntaxFlags.AllowUncHost) != 0)
            {
                //
                // This must remain as the last check before BasicHost type
                //
                if (UncNameHelper.IsValid(pString, start, ref end, StaticNotAny(flags, Flags.ImplicitFile)))
                {
                    if (end - start <= UncNameHelper.MaximumInternetNameLength)
                    {
                        flags |= Flags.UncHostType;
                        if (hasUnicode)
                        {
                            newHost = string.Concat(newHost, new ReadOnlySpan<char>(pString + start, end - start));
                        }
                    }
                }
            }

            // The deal here is that we won't allow '\' host terminator except for the File scheme
            // If we see '\' we try to make it a part of a Basic host
            if (end < length && pString[end] == '\\' && (flags & Flags.HostTypeMask) != Flags.HostNotParsed
                && !StaticIsFile(syntax))
            {
                if (syntax.InFact(UriSyntaxFlags.V1_UnknownUri))
                {
                    err = ParsingError.BadHostName;
                    flags |= Flags.UnknownHostType;
                    return end;
                }
                flags &= ~Flags.HostTypeMask;
            }
            // Here we have checked the syntax up to the end of host
            // The only thing that can cause an exception is the port value
            // Spend some (duplicated) cycles on that.
            else if (end < length && pString[end] == ':')
            {
                if (syntax.InFact(UriSyntaxFlags.MayHavePort))
                {
                    int port = 0;
                    int startPort = end;
                    for (idx = end + 1; idx < length; ++idx)
                    {
                        int val = pString[idx] - '0';
                        if ((uint)val <= ('9' - '0'))
                        {
                            if ((port = (port * 10 + val)) > 0xFFFF)
                                break;
                        }
                        else if (val == ('/' - '0') || val == ('?' - '0') || val == ('#' - '0'))
                        {
                            break;
                        }
                        else
                        {
                            // The second check is to keep compatibility with V1 until the UriParser is registered
                            if (syntax.InFact(UriSyntaxFlags.AllowAnyOtherHost)
                                && syntax.NotAny(UriSyntaxFlags.V1_UnknownUri))
                            {
                                flags &= ~Flags.HostTypeMask;
                                break;
                            }
                            else
                            {
                                err = ParsingError.BadPort;
                                return idx;
                            }
                        }
                    }
                    // check on 0-ffff range
                    if (port > 0xFFFF)
                    {
                        if (syntax.InFact(UriSyntaxFlags.AllowAnyOtherHost))
                        {
                            flags &= ~Flags.HostTypeMask;
                        }
                        else
                        {
                            err = ParsingError.BadPort;
                            return idx;
                        }
                    }

                    if (hasUnicode)
                    {
                        newHost = string.Concat(newHost, new ReadOnlySpan<char>(pString + startPort, idx - startPort));
                    }
                }
                else
                {
                    flags &= ~Flags.HostTypeMask;
                }
            }

            // check on whether nothing has worked out
            if ((flags & Flags.HostTypeMask) == Flags.HostNotParsed)
            {
                //No user info for a Basic hostname
                flags &= ~Flags.HasUserInfo;
                // Some schemes do not allow HostType = Basic (plus V1 almost never understands this issue)
                //
                if (syntax.InFact(UriSyntaxFlags.AllowAnyOtherHost))
                {
                    flags |= Flags.BasicHostType;
                    for (end = idx; end < length; ++end)
                    {
                        if (pString[end] == '/' || (pString[end] == '?' || pString[end] == '#'))
                        {
                            break;
                        }
                    }

                    if (hasUnicode)
                    {
                        // Normalize any other host or do idn
                        try
                        {
                            newHost = UriHelper.NormalizeAndConcat(newHost, new ReadOnlySpan<char>(pString + startInput, end - startInput));
                        }
                        catch (ArgumentException)
                        {
                            err = ParsingError.BadHostName;
                        }
                    }
                }
                else
                {
                    //
                    // ATTN V1 compat: V1 supports hostnames like ".." and ".", and so we do but only for unknown schemes.
                    //
                    if (syntax.InFact(UriSyntaxFlags.V1_UnknownUri))
                    {
                        // Can assert here that the host is not empty so we will set dotFound
                        // at least once or fail before exiting the loop
                        bool dotFound = false;
                        int startOtherHost = idx;
                        for (end = idx; end < length; ++end)
                        {
                            if (dotFound && (pString[end] == '/' || pString[end] == '?' || pString[end] == '#'))
                                break;
                            else if (end < (idx + 2) && pString[end] == '.')
                            {
                                // allow one or two dots
                                dotFound = true;
                            }
                            else
                            {
                                //failure
                                err = ParsingError.BadHostName;
                                flags |= Flags.UnknownHostType;
                                return idx;
                            }
                        }
                        //success
                        flags |= Flags.BasicHostType;

                        if (hasUnicode)
                        {
                            // Normalize any other host
                            try
                            {
                                newHost = UriHelper.NormalizeAndConcat(newHost, new ReadOnlySpan<char>(pString + startOtherHost, end - startOtherHost));
                            }
                            catch (ArgumentException)
                            {
                                err = ParsingError.BadFormat;
                                return idx;
                            }
                        }
                    }
                    else if (syntax.InFact(UriSyntaxFlags.MustHaveAuthority) ||
                             (syntax.InFact(UriSyntaxFlags.MailToLikeUri)))
                    {
                        err = ParsingError.BadHostName;
                        flags |= Flags.UnknownHostType;
                        return idx;
                    }
                }
            }
            return end;
        }

        private static unsafe void CheckAuthorityHelperHandleDnsIri(char* pString, int start, int end,
            bool hasUnicode, ref Flags flags,
            ref string? newHost, ref ParsingError err)
        {
            // comes here only if host has unicode chars and iri is on or idn is allowed

            flags |= Flags.DnsHostType;

            if (hasUnicode)
            {
                ReadOnlySpan<char> host = new ReadOnlySpan<char>(pString + start, end - start);

                if (UriHelper.StripBidiControlCharacters(host, out string? stripped))
                {
                    host = stripped;
                }

                try
                {
                    newHost = UriHelper.NormalizeAndConcat(newHost, host);
                }
                catch (ArgumentException)
                {
                    err = ParsingError.BadHostName;
                }
            }
        }

        //
        // The method checks whether a string needs transformation before going to display or wire
        //
        // Parameters:
        // - escaped   true = treat all valid escape sequences as escaped sequences, false = escape all %
        // - delim     a character signaling the termination of the component being checked
        //
        // When delim=='?', then '#' character is also considered as delimiter additionally to passed '?'.
        //
        // The method pays attention to the dots and slashes so to signal potential Path compression action needed.
        // Even that is not required for other components, the cycles are still spent (little inefficiency)
        //

        internal const char c_DummyChar = (char)0xFFFF;     //An Invalid Unicode character used as a dummy char passed into the parameter
        internal const char c_EOL = (char)0xFFFE;     //An Invalid Unicode character used by CheckCanonical as "no delimiter condition"
        [Flags]
        private enum Check
        {
            None = 0x0,
            EscapedCanonical = 0x1,
            DisplayCanonical = 0x2,
            DotSlashAttn = 0x4,
            DotSlashEscaped = 0x80,
            BackslashInPath = 0x10,
            ReservedFound = 0x20,
            NotIriCanonical = 0x40,
            FoundNonAscii = 0x8
        }

        //
        // Used by ParseRemaining as well by InternalIsWellFormedOriginalString
        //
        private unsafe Check CheckCanonical(char* str, ref int idx, int end, char delim)
        {
            Check res = Check.None;
            bool needsEscaping = false;
            bool foundEscaping = false;
            bool iriParsing = IriParsing;

            char c;
            int i = idx;
            for (; i < end; ++i)
            {
                c = str[i];
                // Control chars usually should be escaped in any case
                if (c <= '\x1F' || (c >= '\x7F' && c <= '\x9F'))
                {
                    needsEscaping = true;
                    foundEscaping = true;
                    res |= Check.ReservedFound;
                }
                else if (c > '~')
                {
                    if (iriParsing)
                    {
                        bool valid = false;
                        res |= Check.FoundNonAscii;

                        if (char.IsHighSurrogate(c))
                        {
                            if ((i + 1) < end)
                            {
                                valid = IriHelper.CheckIriUnicodeRange(c, str[i + 1], out _, true);
                            }
                        }
                        else
                        {
                            valid = IriHelper.CheckIriUnicodeRange(c, true);
                        }
                        if (!valid) res |= Check.NotIriCanonical;
                    }

                    if (!needsEscaping) needsEscaping = true;
                }
                else if (c == delim)
                {
                    break;
                }
                else if (delim == '?' && c == '#' && (_syntax != null && _syntax.InFact(UriSyntaxFlags.MayHaveFragment)))
                {
                    // this is a special case when deciding on Query/Fragment
                    break;
                }
                else if (c == '?')
                {
                    if (IsImplicitFile || (_syntax != null && !_syntax.InFact(UriSyntaxFlags.MayHaveQuery)
                        && delim != c_EOL))
                    {
                        // If found as reserved this char is not suitable for safe unescaped display
                        // Will need to escape it when both escaping and unescaping the string
                        res |= Check.ReservedFound;
                        foundEscaping = true;
                        needsEscaping = true;
                    }
                }
                else if (c == '#')
                {
                    needsEscaping = true;
                    if (IsImplicitFile || (_syntax != null && !_syntax.InFact(UriSyntaxFlags.MayHaveFragment)))
                    {
                        // If found as reserved this char is not suitable for safe unescaped display
                        // Will need to escape it when both escaping and unescaping the string
                        res |= Check.ReservedFound;
                        foundEscaping = true;
                    }
                }
                else if (c == '/' || c == '\\')
                {
                    if ((res & Check.BackslashInPath) == 0 && c == '\\')
                    {
                        res |= Check.BackslashInPath;
                    }
                    if ((res & Check.DotSlashAttn) == 0 && i + 1 != end && (str[i + 1] == '/' || str[i + 1] == '\\'))
                    {
                        res |= Check.DotSlashAttn;
                    }
                }
                else if (c == '.')
                {
                    if ((res & Check.DotSlashAttn) == 0 && i + 1 == end || str[i + 1] == '.' || str[i + 1] == '/'
                        || str[i + 1] == '\\' || str[i + 1] == '?' || str[i + 1] == '#')
                    {
                        res |= Check.DotSlashAttn;
                    }
                }
                else if (((c <= '"' && c != '!') || (c >= '[' && c <= '^') || c == '>'
                        || c == '<' || c == '`'))
                {
                    if (!needsEscaping) needsEscaping = true;

                    // The check above validates only that we have valid IRI characters, which is not enough to
                    // conclude that we have a valid canonical IRI.
                    // If we have an IRI with Flags.HasUnicode, we need to set Check.NotIriCanonical so that the
                    // path, query, and fragment will be validated.
                    if ((_flags & Flags.HasUnicode) != 0)
                    {
                        res |= Check.NotIriCanonical;
                    }
                }
                else if (c >= '{' && c <= '}') // includes '{', '|', '}'
                {
                    needsEscaping = true;
                }
                else if (c == '%')
                {
                    if (!foundEscaping) foundEscaping = true;
                    //try unescape a byte hex escaping
                    if (i + 2 < end && (c = UriHelper.DecodeHexChars(str[i + 1], str[i + 2])) != c_DummyChar)
                    {
                        if (c == '.' || c == '/' || c == '\\')
                        {
                            res |= Check.DotSlashEscaped;
                        }
                        i += 2;
                        continue;
                    }
                    // otherwise we follow to non escaped case
                    if (!needsEscaping)
                    {
                        needsEscaping = true;
                    }
                }
            }

            if (foundEscaping)
            {
                if (!needsEscaping)
                {
                    res |= Check.EscapedCanonical;
                }
            }
            else
            {
                res |= Check.DisplayCanonical;
                if (!needsEscaping)
                {
                    res |= Check.EscapedCanonical;
                }
            }
            idx = i;
            return res;
        }

        //
        // Returns the escaped and canonicalized path string
        // the passed array must be long enough to hold at least
        // canonical unescaped path representation (allocated by the caller)
        //
        private unsafe void GetCanonicalPath(ref ValueStringBuilder dest, UriFormat formatAs)
        {
            if (InFact(Flags.FirstSlashAbsent))
                dest.Append('/');

            if (_info.Offset.Path == _info.Offset.Query)
                return;

            int start = dest.Length;

            int dosPathIdx = SecuredPathIndex;

            // Note that unescaping and then escaping back is not transitive hence not safe.
            // We are vulnerable due to the way the UserEscaped flag is processed.
            // Try to unescape only needed chars.
            if (formatAs == UriFormat.UriEscaped)
            {
                if (InFact(Flags.ShouldBeCompressed))
                {
                    dest.Append(_string.AsSpan(_info.Offset.Path, _info.Offset.Query - _info.Offset.Path));

                    // If the path was found as needed compression and contains escaped characters, unescape only
                    // interesting characters (safe)

                    if (_syntax.InFact(UriSyntaxFlags.UnEscapeDotsAndSlashes) && InFact(Flags.PathNotCanonical)
                        && !IsImplicitFile)
                    {
                        fixed (char* pdest = dest)
                        {
                            int end = dest.Length;
                            UnescapeOnly(pdest, start, ref end, '.', '/',
                                _syntax.InFact(UriSyntaxFlags.ConvertPathSlashes) ? '\\' : c_DummyChar);
                            dest.Length = end;
                        }
                    }
                }
                else
                {
                    //Note: we may produce non escaped Uri characters on the wire
                    if (InFact(Flags.E_PathNotCanonical) && NotAny(Flags.UserEscaped))
                    {
                        ReadOnlySpan<char> str = _string;

                        // Check on not canonical disk designation like C|\, should be rare, rare case
                        if (dosPathIdx != 0 && str[dosPathIdx + _info.Offset.Path - 1] == '|')
                        {
                            char[] chars = str.ToArray();
                            chars[dosPathIdx + _info.Offset.Path - 1] = ':';
                            str = chars;
                        }

                        UriHelper.EscapeString(
                            str.Slice(_info.Offset.Path, _info.Offset.Query - _info.Offset.Path),
                            ref dest, checkExistingEscaped: !IsImplicitFile, UriHelper.UnreservedReservedExceptQuestionMarkHash);
                    }
                    else
                    {
                        dest.Append(_string.AsSpan(_info.Offset.Path, _info.Offset.Query - _info.Offset.Path));
                    }
                }

                // On Unix, escape '\\' in path of file uris to '%5C' canonical form.
                if (!OperatingSystem.IsWindows() && InFact(Flags.BackslashInPath) && _syntax.NotAny(UriSyntaxFlags.ConvertPathSlashes) && _syntax.InFact(UriSyntaxFlags.FileLikeUri) && !IsImplicitFile)
                {
                    // We can't do an in-place escape, create a copy
                    var copy = new ValueStringBuilder(stackalloc char[StackallocThreshold]);
                    copy.Append(dest.AsSpan(start, dest.Length - start));

                    dest.Length = start;

                    UriHelper.EscapeString(copy.AsSpan(), ref dest, checkExistingEscaped: true, UriHelper.UnreservedReserved);
                    start = dest.Length;

                    copy.Dispose();
                }
            }
            else
            {
                dest.Append(_string.AsSpan(_info.Offset.Path, _info.Offset.Query - _info.Offset.Path));

                if (InFact(Flags.ShouldBeCompressed))
                {
                    // If the path was found as needed compression and contains escaped characters,
                    // unescape only interesting characters (safe)

                    if (_syntax.InFact(UriSyntaxFlags.UnEscapeDotsAndSlashes) && InFact(Flags.PathNotCanonical)
                        && !IsImplicitFile)
                    {
                        fixed (char* pdest = dest)
                        {
                            int end = dest.Length;
                            UnescapeOnly(pdest, start, ref end, '.', '/',
                                _syntax.InFact(UriSyntaxFlags.ConvertPathSlashes) ? '\\' : c_DummyChar);
                            dest.Length = end;
                        }
                    }
                }
            }

            // Here we already got output data as copied into dest array
            // We just may need more processing of that data

            //
            // if this URI is using 'non-proprietary' disk drive designation, convert to MS-style
            //
            // (path is already  >= 3 chars if recognized as a DOS-like)
            //
            int offset = start + dosPathIdx;
            if (dosPathIdx != 0 && dest[offset - 1] == '|')
                dest[offset - 1] = ':';

            if (InFact(Flags.ShouldBeCompressed) && dest.Length - offset > 0)
            {
                // It will also convert back slashes if needed
                dest.Length = offset + Compress(dest.RawChars.Slice(offset, dest.Length - offset), _syntax);
                if (dest[start] == '\\')
                    dest[start] = '/';

                // Escape path if requested and found as not fully escaped
                if (formatAs == UriFormat.UriEscaped && NotAny(Flags.UserEscaped) && InFact(Flags.E_PathNotCanonical))
                {
                    //Note: Flags.UserEscaped check is solely based on trusting the user

                    // We can't do an in-place escape, create a copy
                    var copy = new ValueStringBuilder(stackalloc char[StackallocThreshold]);
                    copy.Append(dest.AsSpan(start, dest.Length - start));

                    dest.Length = start;

                    UriHelper.EscapeString(copy.AsSpan(), ref dest, checkExistingEscaped: !IsImplicitFile, UriHelper.UnreservedReservedExceptQuestionMarkHash);
                    start = dest.Length;

                    copy.Dispose();
                }
            }

            if (formatAs != UriFormat.UriEscaped && InFact(Flags.PathNotCanonical))
            {
                UnescapeMode mode;
                switch (formatAs)
                {
                    case V1ToStringUnescape:

                        mode = (InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape)
                            | UnescapeMode.V1ToStringFlag;
                        if (IsImplicitFile)
                            mode &= ~UnescapeMode.Unescape;
                        break;

                    case UriFormat.Unescaped:
                        mode = IsImplicitFile ? UnescapeMode.CopyOnly
                            : UnescapeMode.Unescape | UnescapeMode.UnescapeAll;
                        break;

                    default: // UriFormat.SafeUnescaped

                        mode = InFact(Flags.UserEscaped) ? UnescapeMode.Unescape : UnescapeMode.EscapeUnescape;
                        if (IsImplicitFile)
                            mode &= ~UnescapeMode.Unescape;
                        break;
                }

                if (mode != UnescapeMode.CopyOnly)
                {
                    // We can't do an in-place unescape, create a copy
                    var copy = new ValueStringBuilder(stackalloc char[StackallocThreshold]);
                    copy.Append(dest.AsSpan(start, dest.Length - start));

                    dest.Length = start;
                    fixed (char* pCopy = copy)
                    {
                        UriHelper.UnescapeString(pCopy, 0, copy.Length,
                            ref dest, '?', '#', c_DummyChar,
                            mode,
                            _syntax, isQuery: false);
                    }

                    copy.Dispose();
                }
            }
        }

        // works only with ASCII characters, used to partially unescape path before compressing
        private static unsafe void UnescapeOnly(char* pch, int start, ref int end, char ch1, char ch2, char ch3)
        {
            if (end - start < 3)
            {
                //no chance that something is escaped
                return;
            }

            char* pend = pch + end - 2;
            pch += start;
            char* pnew = null;

        over:

            // Just looking for a interested escaped char
            if (pch >= pend) goto done;
            if (*pch++ != '%') goto over;

            char ch = UriHelper.DecodeHexChars(*pch++, *pch++);
            if (!(ch == ch1 || ch == ch2 || ch == ch3)) goto over;

            // Here we found something and now start copying the scanned chars
            pnew = pch - 2;
            *(pnew - 1) = ch;

        over_new:

            if (pch >= pend) goto done;
            if ((*pnew++ = *pch++) != '%') goto over_new;

            ch = UriHelper.DecodeHexChars((*pnew++ = *pch++), (*pnew++ = *pch++));
            if (!(ch == ch1 || ch == ch2 || ch == ch3))
            {
                goto over_new;
            }

            pnew -= 2;
            *(pnew - 1) = ch;

            goto over_new;

        done:
            pend += 2;

            if (pnew == null)
            {
                //nothing was found
                return;
            }

            //the tail may be already processed
            if (pch == pend)
            {
                end -= (int)(pch - pnew);
                return;
            }

            *pnew++ = *pch++;
            if (pch == pend)
            {
                end -= (int)(pch - pnew);
                return;
            }
            *pnew++ = *pch++;
            end -= (int)(pch - pnew);
        }

        private static void Compress(char[] dest, int start, ref int destLength, UriParser syntax)
        {
            destLength = start + Compress(dest.AsSpan(start, destLength - start), syntax);
        }

        //
        // This will compress any "\" "/../" "/./" "///" "/..../" /XXX.../, etc found in the input
        //
        // The passed syntax controls whether to use aggressive compression or the one specified in RFC 2396
        //
        private static int Compress(Span<char> span, UriParser syntax)
        {
            if (syntax.InFact(UriSyntaxFlags.ConvertPathSlashes))
            {
                span.Replace('\\', '/');
            }

            int slashCount = 0;
            int lastSlash = 0;
            int dotCount = 0;
            int removeSegments = 0;

            for (int i = span.Length - 1; i >= 0; i--)
            {
                char ch = span[i];

                // compress multiple '/' for file URI
                if (ch == '/')
                {
                    ++slashCount;
                }
                else
                {
                    if (slashCount > 1)
                    {
                        // else preserve repeated slashes
                        lastSlash = i + 1;
                    }
                    slashCount = 0;
                }

                if (ch == '.')
                {
                    ++dotCount;
                    continue;
                }
                else if (dotCount != 0)
                {
                    bool skipSegment = syntax.NotAny(UriSyntaxFlags.CanonicalizeAsFilePath)
                        && (dotCount > 2 || ch != '/');

                    // Cases:
                    // /./                  = remove this segment
                    // /../                 = remove this segment, mark next for removal
                    // /....x               = DO NOT TOUCH, leave as is
                    // x.../                = DO NOT TOUCH, leave as is, except for V2 legacy mode
                    if (!skipSegment && ch == '/')
                    {
                        if ((lastSlash == i + dotCount + 1 // "/..../"
                                || (lastSlash == 0 && i + dotCount + 1 == span.Length)) // "/..."
                            && (dotCount <= 2))
                        {
                            //  /./ or /.<eos> or /../ or /..<eos>

                            // span.Remove(i + 1, dotCount + (lastSlash == 0 ? 0 : 1));
                            lastSlash = i + 1 + dotCount + (lastSlash == 0 ? 0 : 1);
                            span.Slice(lastSlash).CopyTo(span.Slice(i + 1));
                            span = span.Slice(0, span.Length - (lastSlash - i - 1));

                            lastSlash = i;
                            if (dotCount == 2)
                            {
                                // We have 2 dots in between like /../ or /..<eos>,
                                // Mark next segment for removal and remove this /../ or /..
                                ++removeSegments;
                            }
                            dotCount = 0;
                            continue;
                        }
                    }
                    // .NET 4.5 no longer removes trailing dots in a path segment x.../  or  x...<eos>
                    dotCount = 0;

                    // Here all other cases go such as
                    // x.[..]y or /.[..]x or (/x.[...][/] && removeSegments !=0)
                }

                // Now we may want to remove a segment because of previous /../
                if (ch == '/')
                {
                    if (removeSegments != 0)
                    {
                        --removeSegments;

                        span.Slice(lastSlash + 1).CopyTo(span.Slice(i + 1));
                        span = span.Slice(0, span.Length - (lastSlash - i));
                    }
                    lastSlash = i;
                }
            }

            if (span.Length != 0 && syntax.InFact(UriSyntaxFlags.CanonicalizeAsFilePath))
            {
                if (slashCount <= 1)
                {
                    if (removeSegments != 0 && span[0] != '/')
                    {
                        //remove first not rooted segment
                        lastSlash++;
                        span.Slice(lastSlash).CopyTo(span);
                        return span.Length - lastSlash;
                    }
                    else if (dotCount != 0)
                    {
                        // If final string starts with a segment looking like .[...]/ or .[...]<eos>
                        // then we remove this first segment
                        if (lastSlash == dotCount || (lastSlash == 0 && dotCount == span.Length))
                        {
                            dotCount += lastSlash == 0 ? 0 : 1;
                            span.Slice(dotCount).CopyTo(span);
                            return span.Length - dotCount;
                        }
                    }
                }
            }

            return span.Length;
        }

        //
        // CombineUri
        //
        //  Given 2 URI strings, combine them into a single resultant URI string
        //
        // Inputs:
        //  <argument>  basePart
        //      Base URI to combine with
        //
        //  <argument>  relativePart
        //      String expected to be relative URI
        //
        // Assumes:
        //  <basePart> is in canonic form
        //
        // Returns:
        //  Resulting combined URI string
        //
        private static string CombineUri(Uri basePart, string relativePart, UriFormat uriFormat)
        {
            //NB: relativePart is ensured as not empty by the caller
            //    Another assumption is that basePart is an AbsoluteUri

            // This method was not optimized for efficiency
            // Means a relative Uri ctor may be relatively slow plus it increases the footprint of the baseUri

            char c1 = relativePart[0];

            //check a special case for the base as DOS path and a rooted relative string
            if (basePart.IsDosPath &&
                (c1 == '/' || c1 == '\\') &&
                (relativePart.Length == 1 || (relativePart[1] != '/' && relativePart[1] != '\\')))
            {
                // take relative part appended to the base string after the drive letter
                int idx = basePart.OriginalString.IndexOf(':');
                if (basePart.IsImplicitFile)
                {
                    return string.Concat(basePart.OriginalString.AsSpan(0, idx + 1), relativePart);
                }

                // The basePart has explicit scheme (could be not file:), take the DOS drive ':' position
                idx = basePart.OriginalString.IndexOf(':', idx + 1);
                return string.Concat(basePart.OriginalString.AsSpan(0, idx + 1), relativePart);
            }

            // Check special case for Unc or absolute path in relativePart when base is FILE
            if (StaticIsFile(basePart.Syntax))
            {
                if (c1 == '\\' || c1 == '/')
                {
                    if (relativePart.Length >= 2 && (relativePart[1] == '\\' || relativePart[1] == '/'))
                    {
                        //Assuming relative is a Unc path and base is a file uri.
                        return basePart.IsImplicitFile ? relativePart : "file:" + relativePart;
                    }

                    // here we got an absolute path in relativePart,
                    // For compatibility with V1.0 parser we restrict the compression scope to Unc Share, i.e. \\host\share\
                    if (basePart.IsUnc)
                    {
                        ReadOnlySpan<char> share = basePart.GetParts(UriComponents.Path | UriComponents.KeepDelimiter, UriFormat.Unescaped);
                        int i = share.Slice(1).IndexOf('/');
                        if (i >= 0)
                        {
                            share = share.Slice(0, i + 1);
                        }

                        if (basePart.IsImplicitFile)
                        {
                            return string.Concat(@"\\", basePart.GetParts(UriComponents.Host, UriFormat.Unescaped), share, relativePart);
                        }

                        return string.Concat("file://", basePart.GetParts(UriComponents.Host, uriFormat), share, relativePart);
                    }
                    // It's not obvious but we've checked (for this relativePart format) that baseUti is nor UNC nor DOS path
                    //
                    // Means base is a Unix style path and, btw, IsImplicitFile cannot be the case either
                    return "file://" + relativePart;
                }
            }

            // If we are here we did not recognize absolute DOS/UNC path for a file: base uri
            // Note that DOS path may still happen in the relativePart and if so it may override the base uri scheme.

            bool convBackSlashes = basePart.Syntax.InFact(UriSyntaxFlags.ConvertPathSlashes);

            string? left;

            // check for network or local absolute path
            if (c1 == '/' || (c1 == '\\' && convBackSlashes))
            {
                if (relativePart.Length >= 2 && relativePart[1] == '/')
                {
                    // got an authority in relative path and the base scheme is not file (checked)
                    return basePart.Scheme + ':' + relativePart;
                }

                // Got absolute relative path, and the base is not FILE nor a DOS path (checked at the method start)
                if (basePart.HostType == Flags.IPv6HostType)
                {
                    left = $"{basePart.GetParts(UriComponents.Scheme | UriComponents.UserInfo, uriFormat)}[{basePart.DnsSafeHost}]{basePart.GetParts(UriComponents.KeepDelimiter | UriComponents.Port, uriFormat)}";
                }
                else
                {
                    left = basePart.GetParts(UriComponents.SchemeAndServer | UriComponents.UserInfo, uriFormat);
                }

                return convBackSlashes && c1 == '\\' ?
                    string.Concat(left, "/", relativePart.AsSpan(1)) :
                    left + relativePart;
            }

            // Here we got a relative path
            // Need to run path Compression because this is how relative Uri combining works

            // Take the base part path up to and including the last slash
            left = basePart.GetParts(UriComponents.Path | UriComponents.KeepDelimiter,
                basePart.IsImplicitFile ? UriFormat.Unescaped : uriFormat);
            int length = left.Length;
            char[] path = new char[length + relativePart.Length];

            if (length > 0)
            {
                left.CopyTo(0, path, 0, length);
                while (length > 0)
                {
                    if (path[--length] == '/')
                    {
                        ++length;
                        break;
                    }
                }
            }

            //Append relative path to the result
            relativePart.CopyTo(0, path, length, relativePart.Length);

            // Split relative on path and extra (for compression)
            c1 = basePart.Syntax.InFact(UriSyntaxFlags.MayHaveQuery) ? '?' : c_DummyChar;

            // The  implicit file check is to avoid a fragment in the implicit file combined uri.
            char c2 = (!basePart.IsImplicitFile && basePart.Syntax.InFact(UriSyntaxFlags.MayHaveFragment)) ? '#' :
                c_DummyChar;
            ReadOnlySpan<char> extra = string.Empty;

            // assuming c_DummyChar may not happen in an unicode uri string
            if (!(c1 == c_DummyChar && c2 == c_DummyChar))
            {
                int i = 0;
                for (; i < relativePart.Length; ++i)
                {
                    if (path[length + i] == c1 || path[length + i] == c2)
                    {
                        break;
                    }
                }
                if (i == 0)
                {
                    extra = relativePart;
                }
                else if (i < relativePart.Length)
                {
                    extra = relativePart.AsSpan(i);
                }
                length += i;
            }
            else
            {
                length += relativePart.Length;
            }

            // Take the base part up to the path
            if (basePart.HostType == Flags.IPv6HostType)
            {
                if (basePart.IsImplicitFile)
                {
                    left = @"\\[" + basePart.DnsSafeHost + ']';
                }
                else
                {
                    left = basePart.GetParts(UriComponents.Scheme | UriComponents.UserInfo, uriFormat)
                            + '[' + basePart.DnsSafeHost + ']'
                            + basePart.GetParts(UriComponents.KeepDelimiter | UriComponents.Port, uriFormat);
                }
            }
            else
            {
                if (basePart.IsImplicitFile)
                {
                    if (basePart.IsDosPath)
                    {
                        // The FILE DOS path comes as /c:/path, we have to exclude first 3 chars from compression
                        Compress(path, 3, ref length, basePart.Syntax);
                        return string.Concat(path.AsSpan(1, length - 1), extra);
                    }
                    else if (!OperatingSystem.IsWindows() && basePart.IsUnixPath)
                    {
                        left = basePart.GetParts(UriComponents.Host, UriFormat.Unescaped);
                    }
                    else
                    {
                        left = @"\\" + basePart.GetParts(UriComponents.Host, UriFormat.Unescaped);
                    }
                }
                else
                {
                    left = basePart.GetParts(UriComponents.SchemeAndServer | UriComponents.UserInfo, uriFormat);
                }
            }
            //compress the path
            Compress(path, basePart.SecuredPathIndex, ref length, basePart.Syntax);
            return string.Concat(left, path.AsSpan(0, length), extra);
        }

        //
        // PathDifference
        //
        //  Performs the relative path calculation for MakeRelative()
        //
        // Inputs:
        //  <argument>  path1
        //  <argument>  path2
        //      Paths for which we calculate the difference
        //
        //  <argument>  compareCase
        //      False if we consider characters that differ only in case to be
        //      equal
        //
        // Returns:
        //  A string which is the relative path difference between <path1> and
        //  <path2> such that if <path1> and the calculated difference are used
        //  as arguments to Combine(), <path2> is returned
        //
        // Throws:
        //  Nothing
        //
        private static string PathDifference(string path1, string path2, bool compareCase)
        {
            int i;
            int si = -1;

            for (i = 0; (i < path1.Length) && (i < path2.Length); ++i)
            {
                if ((path1[i] != path2[i])
                    && (compareCase
                        || (char.ToLowerInvariant(path1[i])
                            != char.ToLowerInvariant(path2[i]))))
                {
                    break;
                }
                else if (path1[i] == '/')
                {
                    si = i;
                }
            }

            if (i == 0)
            {
                return path2;
            }
            if ((i == path1.Length) && (i == path2.Length))
            {
                return string.Empty;
            }

            StringBuilder relPath = new StringBuilder();
            // Walk down several dirs
            for (; i < path1.Length; ++i)
            {
                if (path1[i] == '/')
                {
                    relPath.Append("../");
                }
            }
            // Same path except that path1 ended with a file name and path2 didn't
            if (relPath.Length == 0 && path2.Length - 1 == si)
                return "./"; // Truncate the file name
            return relPath.Append(path2.AsSpan(si + 1)).ToString();
        }

        //
        // MakeRelative (toUri)
        //
        //  Return a relative path which when applied to this Uri would create the
        //  resulting Uri <toUri>
        //
        // Inputs:
        //  <argument>  toUri
        //      Uri to which we calculate the transformation from this Uri
        //
        // Returns:
        //  If the 2 Uri are common except for a relative path difference, then that
        //  difference, else the display name of this Uri
        //
        // Throws:
        //  ArgumentNullException, InvalidOperationException
        //
        [Obsolete("Uri.MakeRelative has been deprecated. Use MakeRelativeUri(Uri uri) instead.")]
        public string MakeRelative(Uri toUri)
        {
            ArgumentNullException.ThrowIfNull(toUri);

            if (IsNotAbsoluteUri || toUri.IsNotAbsoluteUri)
                throw new InvalidOperationException(SR.net_uri_NotAbsolute);

            if ((Scheme == toUri.Scheme) && (Host == toUri.Host) && (Port == toUri.Port))
                return PathDifference(AbsolutePath, toUri.AbsolutePath, !IsUncOrDosPath);

            return toUri.ToString();
        }

        /// <internalonly/>
        [Obsolete("Uri.Canonicalize has been deprecated and is not supported.")]
        protected virtual void Canonicalize()
        {
            // this method if suppressed by the derived class
            // would lead to suppressing of a path compression
            // It does not make much sense and violates Fxcop on calling a virtual method in the ctor.
            // Should be deprecated and removed asap.
        }

        /// <internalonly/>
        [Obsolete("Uri.Parse has been deprecated and is not supported.")]
        protected virtual void Parse()
        {
            // this method if suppressed by the derived class
            // would lead to an unconstructed Uri instance.
            // It does not make any sense and violates Fxcop on calling a virtual method in the ctor.
            // Should be deprecated and removed asap.
        }

        /// <internalonly/>
        [Obsolete("Uri.Escape has been deprecated and is not supported.")]
        protected virtual void Escape()
        {
            // this method if suppressed by the derived class
            // would lead to the same effect as dontEscape=true.
            // It does not make much sense and violates Fxcop on calling a virtual method in the ctor.
            // Should be deprecated and removed asap.
        }

        //
        // Unescape
        //
        //  Convert any escape sequences in <path>. Escape sequences can be
        //  hex encoded reserved characters (e.g. %40 == '@') or hex encoded
        //  UTF-8 sequences (e.g. %C4%D2 == 'Latin capital Ligature Ij')
        //
        /// <internalonly/>
        [Obsolete("Uri.Unescape has been deprecated. Use GetComponents() or Uri.UnescapeDataString() to unescape a Uri component or a string.")]
        protected virtual string Unescape(string path)
        {
            // This method is dangerous since it gives path unescaping control
            // to the derived class without any permission demand.
            // Should be deprecated and removed asap.

            char[] dest = new char[path.Length];
            int count = 0;
            dest = UriHelper.UnescapeString(path, 0, path.Length, dest, ref count, c_DummyChar, c_DummyChar,
                c_DummyChar, UnescapeMode.Unescape | UnescapeMode.UnescapeAll, null, false);
            return new string(dest, 0, count);
        }

        [Obsolete("Uri.EscapeString has been deprecated. Use GetComponents() or Uri.EscapeDataString to escape a Uri component or a string.")]
        protected static string EscapeString(string? str) =>
            str is null ? string.Empty :
            UriHelper.EscapeString(str, checkExistingEscaped: true, UriHelper.UnreservedReservedExceptQuestionMarkHash);

        //
        // CheckSecurity
        //
        //  Check for any invalid or problematic character sequences
        //
        /// <internalonly/>
        [Obsolete("Uri.CheckSecurity has been deprecated and is not supported.")]
        protected virtual void CheckSecurity()
        {
            // This method just does not make sense
            // Should be deprecated and removed asap.
        }

        //
        // IsReservedCharacter
        //
        //  Determine whether a character is part of the reserved set
        //
        // Returns:
        //  true if <character> is reserved else false
        //
        /// <internalonly/>
        [Obsolete("Uri.IsReservedCharacter has been deprecated and is not supported.")]
        protected virtual bool IsReservedCharacter(char character)
        {
            // This method just does not make sense as protected virtual
            // It should go public static asap

            return (character == ';')
                || (character == '/')
                || (character == ':')
                || (character == '@')   // OK FS char
                || (character == '&')
                || (character == '=')
                || (character == '+')   // OK FS char
                || (character == '$')   // OK FS char
                || (character == ',')
                ;
        }

        //
        // IsExcludedCharacter
        //
        //  Determine if a character should be excluded from a URI and therefore be
        //  escaped
        //
        // Returns:
        //  true if <character> should be escaped else false
        //
        /// <internalonly/>
        [Obsolete("Uri.IsExcludedCharacter has been deprecated and is not supported.")]
        protected static bool IsExcludedCharacter(char character)
        {
            // This method just does not make sense as protected
            // It should go public static asap

            //
            // the excluded characters...
            //

            return (character <= 0x20)
                || (character >= 0x7f)
                || (character == '<')
                || (character == '>')
                || (character == '#')
                || (character == '%')
                || (character == '"')

                //
                // the 'unwise' characters...
                //

                || (character == '{')
                || (character == '}')
                || (character == '|')
                || (character == '\\')
                || (character == '^')
                || (character == '[')
                || (character == ']')
                || (character == '`')
                ;
        }

        //
        // IsBadFileSystemCharacter
        //
        //  Determine whether a character would be an invalid character if used in
        //  a file system name. Note, this is really based on NTFS rules
        //
        // Returns:
        //  true if <character> would be a treated as a bad file system character
        //  else false
        //
        [Obsolete("Uri.IsBadFileSystemCharacter has been deprecated and is not supported.")]
        protected virtual bool IsBadFileSystemCharacter(char character)
        {
            // This method just does not make sense as protected virtual
            // It should go public static asap

            return (character < 0x20)
                || (character == ';')
                || (character == '/')
                || (character == '?')
                || (character == ':')
                || (character == '&')
                || (character == '=')
                || (character == ',')
                || (character == '*')
                || (character == '<')
                || (character == '>')
                || (character == '"')
                || (character == '|')
                || (character == '\\')
                || (character == '^')
                ;
        }

        //Used by UriBuilder
        internal bool HasAuthority
        {
            get
            {
                return InFact(Flags.AuthorityFound);
            }
        }
    } // class Uri
} // namespace System
