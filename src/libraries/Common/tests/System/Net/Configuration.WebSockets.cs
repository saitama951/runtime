// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

namespace System.Net.Test.Common
{
    public static partial class Configuration
    {
        public static partial class WebSockets
        {
            public static string ProxyServerUri => GetValue("DOTNET_TEST_WEBSOCKETPROXYSERVERURI");

            public static string Host => GetValue("DOTNET_TEST_WEBSOCKETHOST", DefaultAzureServer);

            public static string SecureHost => GetValue("DOTNET_TEST_SECUREWEBSOCKETHOST", DefaultAzureServer);

            private const string EchoHandler = "WebSocket/EchoWebSocket.ashx";
            private const string EchoHeadersHandler = "WebSocket/EchoWebSocketHeaders.ashx";

            public static readonly Uri RemoteEchoServer = new Uri("ws://" + Host + "/" + EchoHandler);
            public static readonly Uri SecureRemoteEchoServer = new Uri("wss://" + SecureHost + "/" + EchoHandler);

            public static readonly Uri RemoteEchoHeadersServer = new Uri("ws://" + Host + "/" + EchoHeadersHandler);
            public static readonly Uri SecureRemoteEchoHeadersServer = new Uri("wss://" + SecureHost + "/" + EchoHeadersHandler);

            public static object[][] GetEchoServers()
            {
                if (PlatformDetection.IsFirefox)
                {
                    // https://github.com/dotnet/runtime/issues/101115
                    return new object[][] {
                        // [ActiveIssue("https://github.com/dotnet/runtime/issues/110578)]
                        // new object[] { RemoteEchoServer },

                    };
                }
                return new object[][] {
                    // [ActiveIssue("https://github.com/dotnet/runtime/issues/110578)]
                    // new object[] { RemoteEchoServer },
                    new object[] { SecureRemoteEchoServer },
                };
            }

            public static object[][] GetEchoHeadersServers()
            {
                if (PlatformDetection.IsFirefox)
                {
                    // https://github.com/dotnet/runtime/issues/101115
                    return new object[][] {
                        // [ActiveIssue("https://github.com/dotnet/runtime/issues/110578)]
                        // new object[] { RemoteEchoHeadersServer },
                    };
                }
                return new object[][] {
                    // [ActiveIssue("https://github.com/dotnet/runtime/issues/110578)]
                    // new object[] { RemoteEchoHeadersServer },
                    new object[] { SecureRemoteEchoHeadersServer },
                };
            }
        }
    }
}
