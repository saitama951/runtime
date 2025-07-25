// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#nullable enable

using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;
using Microsoft.NET.Sdk.WebAssembly;
using Xunit;
using Xunit.Abstractions;
using Xunit.Sdk;

namespace Wasm.Build.Tests;

// For projects using WasmAppBuilder
// ToDo: REMOVE, use WasmSdkBasedProjectProvider only
public abstract class ProjectProviderBase(ITestOutputHelper _testOutput, string? _projectDir)
{
    public static string WasmAssemblyExtension = BuildTestBase.s_buildEnv.UseWebcil ? ".wasm" : ".dll";
    protected const string s_dotnetVersionHashRegex = @"\.(?<hash>[a-zA-Z0-9]+)\.";

    private const string s_runtimePackPathPattern = "\\*\\* MicrosoftNetCoreAppRuntimePackDir : '([^']*)'";
    private static Regex s_runtimePackPathRegex = new Regex(s_runtimePackPathPattern);
    private static string[] s_dotnetExtensionsToIgnore = new[]
    {
        ".gz",
        ".br"
    };

    public string? ProjectDir { get; set; } = _projectDir;
    protected ITestOutputHelper _testOutput = new TestOutputWrapper(_testOutput);
    protected BuildEnvironment _buildEnv = BuildTestBase.s_buildEnv;
    protected abstract string BundleDirName { get; }

    public bool IsFingerprintingEnabled => EnvironmentVariables.UseFingerprinting;

    // Returns the actual files on disk
    public IReadOnlyDictionary<string, DotNetFileName> AssertBasicBundle(AssertBundleOptions assertOptions)
    {
        EnsureProjectDirIsSet();
        var dotnetFiles = FindAndAssertDotnetFiles(assertOptions);

        TestUtils.AssertFilesExist(assertOptions.BinFrameworkDir,
                                   new[] { "System.Private.CoreLib.dll" },
                                   expectToExist: IsFingerprintingEnabled ? false : !BuildTestBase.UseWebcil);
        TestUtils.AssertFilesExist(assertOptions.BinFrameworkDir,
                                   new[] { "System.Private.CoreLib.wasm" },
                                   expectToExist: IsFingerprintingEnabled ? false : BuildTestBase.UseWebcil);

        var bootJson = AssertBootJson(assertOptions);

        // icu
        if (assertOptions.AssertIcuAssets)
        {
            AssertIcuAssets(assertOptions, bootJson);
        }
        else
        {
            _testOutput.WriteLine("Skipping asserting icu assets");
        }

        // symbols
        if (assertOptions.AssertSymbolsFile)
        {
            _testOutput.WriteLine("Skipping asserting symbols file");
            AssertDotNetJsSymbols(assertOptions);
        }

        return dotnetFiles;
    }

    public IReadOnlyDictionary<string, DotNetFileName> FindAndAssertDotnetFiles(AssertBundleOptions assertOptions)
    {
        EnsureProjectDirIsSet();
        return FindAndAssertDotnetFiles(binFrameworkDir: assertOptions.BinFrameworkDir,
                                        assertOptions,
                                        superSet: GetAllKnownDotnetFilesToFingerprintMap(assertOptions),
                                        expected: GetDotNetFilesExpectedSet(assertOptions)
                                        );
    }

    protected abstract IReadOnlyDictionary<string, bool> GetAllKnownDotnetFilesToFingerprintMap(AssertBundleOptions assertOptions);
    protected abstract IReadOnlySet<string> GetDotNetFilesExpectedSet(AssertBundleOptions assertOptions);

    public IReadOnlyDictionary<string, DotNetFileName> FindAndAssertDotnetFiles(
        string binFrameworkDir,
        AssertBundleOptions assertOptions,
        IReadOnlyDictionary<string, bool> superSet,
        IReadOnlySet<string> expected)
    {
        var expectFingerprintOnDotnetJs = assertOptions.ExpectDotnetJsFingerprinting;
        EnsureProjectDirIsSet();
        var actual = new SortedDictionary<string, DotNetFileName>();

        if (!Directory.Exists(binFrameworkDir))
            throw new XunitException($"Could not find bundle directory {binFrameworkDir}");
        IList<string> dotnetFiles = Directory.EnumerateFiles(binFrameworkDir,
                                                             "dotnet.*",
                                                             SearchOption.TopDirectoryOnly)
                                                .Order()
                                                .ToList();
        
        var comparisonLogging = new List<string>();

        foreach ((string expectedFilename, bool expectFingerprint) in superSet.OrderByDescending(kvp => kvp.Key))
        {
            string prefix = Path.GetFileNameWithoutExtension(expectedFilename);
            string extension = Path.GetExtension(expectedFilename).Substring(1);

            dotnetFiles = dotnetFiles
                .Where(actualFile =>
                {
                    if (s_dotnetExtensionsToIgnore.Contains(Path.GetExtension(actualFile)))
                        return false;

                    string actualFilename = Path.GetFileName(actualFile);
                    comparisonLogging.Add($"Comparing {expectedFilename} with {actualFile}, expectFingerprint: {expectFingerprint}");
                    if (ShouldCheckFingerprint(expectedFilename: expectedFilename,
                                               expectFingerprintOnDotnetJs: expectFingerprintOnDotnetJs,
                                               expectFingerprintForThisFile: expectFingerprint))
                    {
                        string pattern = $"^{prefix}{s_dotnetVersionHashRegex}{extension}$";
                        var match = Regex.Match(actualFilename, pattern);
                        if (!match.Success)
                            return true;

                        actual[expectedFilename] = new(ExpectedFilename: expectedFilename,
                                                       Hash: match.Groups[1].Value,
                                                       ActualPath: actualFile);
                    }
                    else
                    {
                        if (actualFilename != expectedFilename)
                            return true;

                        actual[expectedFilename] = new(ExpectedFilename: expectedFilename,
                                                       Hash: null,
                                                       ActualPath: actualFile);
                    }

                    return false;
                }).ToList();
        }

        if (dotnetFiles.Any())
        {
            foreach (var message in comparisonLogging)
            {
                _testOutput.WriteLine(message);
            }
            _testOutput.WriteLine($"Accepted count: {actual.Count}");
            foreach (var kvp in actual)
            {
                _testOutput.WriteLine($"Accepted: \t[{kvp.Key}] = {kvp.Value}");
            }

            throw new XunitException($"Found unknown files in {binFrameworkDir}:{Environment.NewLine}    " +
                    $"{string.Join($"{Environment.NewLine}  ", dotnetFiles.Select(f => Path.GetRelativePath(binFrameworkDir, f)))}{Environment.NewLine}" +
                    $"Add these to {nameof(GetAllKnownDotnetFilesToFingerprintMap)} method{Environment.NewLine}" + 
                    $"Expected {string.Join($"{Environment.NewLine}  ", expected)}{Environment.NewLine}" + 
                    $"Options {assertOptions} {Environment.NewLine}"
                    );
        }

        if (expected is not null)
            AssertDotNetFilesSet(assertOptions, expected, superSet, actual, expectFingerprintOnDotnetJs, binFrameworkDir);
        return actual;
    }

    private void AssertDotNetFilesSet(
        AssertBundleOptions assertOptions,
        IReadOnlySet<string> expected,
        IReadOnlyDictionary<string, bool> superSet,
        IReadOnlyDictionary<string, DotNetFileName> actualReadOnly,
        bool? expectFingerprintOnDotnetJs,
        string bundleDir)
    {
        EnsureProjectDirIsSet();

        var actual = new Dictionary<string, DotNetFileName>(actualReadOnly);
        foreach (string expectedFilename in expected)
        {
            bool expectFingerprint = superSet[expectedFilename];

            Assert.True(actual.ContainsKey(expectedFilename), $"Could not find {expectedFilename} in bundle directory: {bundleDir}. Actual files on disk: {string.Join(", ", actual.Keys)} Options {assertOptions}");

            // Check that the version and hash are present or not present as expected
            if (ShouldCheckFingerprint(expectedFilename: expectedFilename,
                                       expectFingerprintOnDotnetJs: expectFingerprintOnDotnetJs,
                                       expectFingerprintForThisFile: expectFingerprint))
            {
                if (string.IsNullOrEmpty(actual[expectedFilename].Hash))
                    throw new XunitException($"Expected hash in filename: {actual[expectedFilename].ActualPath} Options {assertOptions}");
            }
            else
            {
                if (!string.IsNullOrEmpty(actual[expectedFilename].Hash))
                    throw new XunitException($"Expected no hash in filename: {actual[expectedFilename].ActualPath} Options {assertOptions}");
            }
            actual.Remove(expectedFilename);
        }

        if (actual.Any())
        {
            var actualFileNames = actual.Values.Select(x => x.ActualPath).Order();
            throw new XunitException($"Found unexpected files: {string.Join(", ", actualFileNames)} Options {assertOptions}");
        }
    }

    public void CompareStat(IDictionary<string, FileStat> oldStat, IDictionary<string, FileStat> newStat, IDictionary<string, (string fullPath, bool unchanged)> expected)
    {
        StringBuilder msg = new();
        foreach (var expect in expected)
        {
            if (!oldStat.TryGetValue(expect.Key, out FileStat? oldFs))
            {
                msg.AppendLine($"Could not find an entry for {expect.Key} in old files");
                continue;
            }

            if (!newStat.TryGetValue(expect.Key, out FileStat? newFs))
            {
                msg.AppendLine($"Could not find an entry for {expect.Key} in new files");
                continue;
            }

            // files never existed existed => no change
            // fingerprinting is enabled => can't compare paths
            bool actualUnchanged = (!oldFs.Exists && !newFs.Exists) ||
                IsFingerprintingEnabled && (oldFs.Length == newFs.Length && oldFs.LastWriteTimeUtc == newFs.LastWriteTimeUtc) ||
                !IsFingerprintingEnabled && oldFs == newFs;

            if (expect.Value.unchanged && !actualUnchanged)
            {
                msg.AppendLine($"[Expected unchanged file: {expect.Key}]{Environment.NewLine}" +
                               $"   old: {oldFs}{Environment.NewLine}" +
                               $"   new: {newFs}");
            }
            else if (!expect.Value.unchanged && actualUnchanged)
            {
                msg.AppendLine($"[Expected changed file: {expect.Key}]{Environment.NewLine}" +
                               $"   {newFs}");
            }
        }

        if (msg.Length > 0)
            throw new XunitException($"CompareStat failed:{Environment.NewLine}{msg}");
    }

    public IDictionary<string, FileStat> StatFiles(IDictionary<string, (string fullPath, bool unchanged)> pathsDict)
    {
        Dictionary<string, FileStat> table = new();
        foreach (var fileInfo in pathsDict)
        {
            string file = fileInfo.Value.fullPath;
            string nameNoFingerprinting = fileInfo.Key;
            bool exists = File.Exists(file);
            if (exists)
            {
                table.Add(nameNoFingerprinting, new FileStat(FullPath: file, Exists: true, LastWriteTimeUtc: File.GetLastWriteTimeUtc(file), Length: new FileInfo(file).Length));
            }
            else
            {
                table.Add(nameNoFingerprinting, new FileStat(FullPath: file, Exists: false, LastWriteTimeUtc: DateTime.MinValue, Length: 0));
            }
        }

        return table;
    }

    public IDictionary<string, FileStat> StatFilesAfterRebuild(IDictionary<string, (string fullPath, bool unchanged)> pathsDict)
    {
        if (!IsFingerprintingEnabled)
            return StatFiles(pathsDict);

        // files are expected to be fingerprinted, so we cannot rely on the paths that come with pathsDict, an update is needed
        Dictionary<string, FileStat> table = new();
        foreach (var fileInfo in pathsDict)
        {
            string file = fileInfo.Value.fullPath;
            string nameNoFingerprinting = fileInfo.Key;
            string[] filesMatchingName = GetFilesMatchingNameConsideringFingerprinting(file, nameNoFingerprinting);
            if (filesMatchingName.Length > 1)
            {
                string? fileMatch = filesMatchingName.FirstOrDefault(f => f != file);
                if (fileMatch != null)
                {
                    table.Add(nameNoFingerprinting, new FileStat(FullPath: fileMatch, Exists: true, LastWriteTimeUtc: File.GetLastWriteTimeUtc(fileMatch), Length: new FileInfo(fileMatch).Length));
                }
            }
            if (filesMatchingName.Length == 0 || (filesMatchingName.Length == 1 && !File.Exists(file)))
            {
                table.Add(nameNoFingerprinting, new FileStat(FullPath: file, Exists: false, LastWriteTimeUtc: DateTime.MinValue, Length: 0));
            }
            if (filesMatchingName.Length == 1 && File.Exists(file))
            {
                table.Add(nameNoFingerprinting, new FileStat(FullPath: file, Exists: true, LastWriteTimeUtc: File.GetLastWriteTimeUtc(file), Length: new FileInfo(file).Length));
            }
        }
        return table;
    }

    private string[] GetFilesMatchingNameConsideringFingerprinting(string filePath, string nameNoFingerprinting)
    {
        var directory = Path.GetDirectoryName(filePath);
        if (directory == null)
            return Array.Empty<string>();

        string fileNameWithoutExtensionAndFingerprinting = Path.GetFileNameWithoutExtension(nameNoFingerprinting);
        string fileExtension = Path.GetExtension(filePath);

        // search for files that match the name in the directory, skipping fingerprinting
        string[] files = Directory.GetFiles(directory, $"{fileNameWithoutExtensionAndFingerprinting}*{fileExtension}");

        // filter files with a single fingerprint segment, e.g. "dotnet*.js" should not catch "dotnet.native.d1au9i.js" but should catch "dotnet.js"
        string pattern = $@"^{Regex.Escape(fileNameWithoutExtensionAndFingerprinting)}(\.[^.]+)?{Regex.Escape(fileExtension)}$";
        var tmp = files.Where(f => Regex.IsMatch(Path.GetFileName(f), pattern)).Where(f => !f.Contains("dotnet.boot")).ToArray();
        return tmp;
    }

    public IDictionary<string, (string fullPath, bool unchanged)> GetFilesTable(bool unchanged, params string[] baseDirs)
    {
        var dict = new Dictionary<string, (string fullPath, bool unchanged)>();
        foreach (var baseDir in baseDirs)
        {
            foreach (var file in Directory.EnumerateFiles(baseDir, "*", new EnumerationOptions { RecurseSubdirectories = true }))
                dict[Path.GetFileName(file)] = (file, unchanged);
        }

        return dict;
    }

    public IDictionary<string, (string fullPath, bool unchanged)> GetFilesTable(string projectName, bool isAOT, BuildPaths paths, bool unchanged)
    {
        List<string> files = new()
        {
            Path.Combine(paths.BinDir, "publish", BundleDirName, "_framework", $"{projectName}{WasmAssemblyExtension}"),
            Path.Combine(paths.ObjWasmDir, "driver.o"),
            Path.Combine(paths.ObjWasmDir, "runtime.o"),
            Path.Combine(paths.ObjWasmDir, "corebindings.o"),
            Path.Combine(paths.ObjWasmDir, "pinvoke.o"),

            Path.Combine(paths.ObjWasmDir, "icall-table.h"),
            Path.Combine(paths.ObjWasmDir, "pinvoke-table.h"),
            Path.Combine(paths.ObjWasmDir, "driver-gen.c"),

            Path.Combine(paths.BinFrameworkDir, "dotnet.native.wasm"),
            Path.Combine(paths.BinFrameworkDir, "dotnet.native.js"),
        };

        if (isAOT)
        {
            files.AddRange(new[]
            {
                Path.Combine(paths.ObjWasmDir, $"{projectName}.dll.bc"),
                Path.Combine(paths.ObjWasmDir, $"{projectName}.dll.o"),

                Path.Combine(paths.ObjWasmDir, $"System.Private.CoreLib.dll.bc"),
                Path.Combine(paths.ObjWasmDir, $"System.Private.CoreLib.dll.o"),
            });
        }

        var dict = new Dictionary<string, (string fullPath, bool unchanged)>();
        foreach (var file in files)
            dict[Path.GetFileName(file)] = (file, unchanged);

        // those files do not change on re-link
        dict["dotnet.js.map"]=(Path.Combine(paths.BinFrameworkDir, "dotnet.js.map"), true);
        dict["dotnet.runtime.js"]=(Path.Combine(paths.BinFrameworkDir, "dotnet.runtime.js"), true);
        dict["dotnet.runtime.js.map"]=(Path.Combine(paths.BinFrameworkDir, "dotnet.runtime.js.map"), true);

        if (IsFingerprintingEnabled)
        {
            string bootJsonPath = GetBootConfigPath(paths.BinFrameworkDir, "dotnet.js");
            BootJsonData bootJson = GetBootJson(bootJsonPath);
            AssetsData assets = (AssetsData)bootJson.resources;
            var keysToUpdate = new List<string>();
            var updates = new List<(string oldKey, string newKey, (string fullPath, bool unchanged) value)>();
            List<GeneralAsset> allAssemblies = [..assets.coreAssembly, ..assets.assembly];

            foreach (var expectedItem in dict)
            {
                string filename = Path.GetFileName(expectedItem.Value.fullPath);
                string? expectedFingerprintedItem = filename switch
                {
                    "dotnet.runtime.js" => assets.jsModuleRuntime?.SingleOrDefault()?.name,
                    "dotnet.native.js" => assets.jsModuleNative?.SingleOrDefault()?.name,
                    "dotnet.native.wasm" => assets.wasmNative?.SingleOrDefault()?.name,
                    _ => filename == $"{projectName}{WasmAssemblyExtension}"
                        ? allAssemblies?.SingleOrDefault(a => a.virtualPath == $"{projectName}{WasmAssemblyExtension}")?.name
                        : null
                };

                if (string.IsNullOrEmpty(expectedFingerprintedItem))
                    continue;

                if (filename != expectedFingerprintedItem)
                {
                    string newKey = Path.Combine(
                        Path.GetDirectoryName(expectedItem.Value.fullPath) ?? "",
                        expectedFingerprintedItem
                    );
                    dict[filename] = (newKey, expectedItem.Value.unchanged);
                }
            }
        }
        return dict;
    }

    public bool ShouldCheckFingerprint(string expectedFilename, bool? expectFingerprintOnDotnetJs, bool expectFingerprintForThisFile)
        => IsFingerprintingEnabled && ((expectedFilename == "dotnet.js" && expectFingerprintOnDotnetJs == true) || expectFingerprintForThisFile);


    public static void AssertRuntimePackPath(string buildOutput, string targetFramework, RuntimeVariant runtimeType = RuntimeVariant.SingleThreaded)
    {
        var match = s_runtimePackPathRegex.Match(buildOutput);
        if (!match.Success || match.Groups.Count != 2)
            throw new XunitException($"Could not find the pattern in the build output: '{s_runtimePackPathPattern}'.{Environment.NewLine}Build output: {buildOutput}");

        string expectedRuntimePackDir = BuildTestBase.s_buildEnv.GetRuntimePackDir(targetFramework, runtimeType);
        string actualPath = match.Groups[1].Value;
        if (string.Compare(actualPath, expectedRuntimePackDir) != 0)
            throw new XunitException($"Runtime pack path doesn't match.{Environment.NewLine}Expected: '{expectedRuntimePackDir}'{Environment.NewLine}Actual:   '{actualPath}'");
    }

    public static void AssertDotNetJsSymbols(AssertBundleOptions assertOptions)
    {
        TestUtils.AssertFilesExist(assertOptions.BinFrameworkDir, new[] { "dotnet.native.js.symbols" }, expectToExist: assertOptions.ExpectSymbolsFile);

        if (assertOptions.BuildOptions.ExpectedFileType == NativeFilesType.FromRuntimePack)
        {
            TestUtils.AssertFile(
                    Path.Combine(BuildTestBase.s_buildEnv.GetRuntimeNativeDir(assertOptions.BuildOptions.TargetFramework, assertOptions.BuildOptions.RuntimeType), "dotnet.native.js.symbols"),
                    Path.Combine(assertOptions.BinFrameworkDir, "dotnet.native.js.symbols"),
                    same: true);
        }
    }

    public void AssertIcuAssets(AssertBundleOptions assertOptions, BootJsonData bootJson)
    {
        AssetsData assets = (AssetsData)bootJson.resources;

        List<string> expected = new();
        switch (assertOptions.BuildOptions.GlobalizationMode)
        {
            case GlobalizationMode.Invariant:
                break;
            case GlobalizationMode.FullIcu:
                expected.Add("icudt.dat");
                break;
            case GlobalizationMode.Custom:
                if (string.IsNullOrEmpty(assertOptions.BuildOptions.CustomIcuFile))
                    throw new ArgumentException("WasmBuildTest is invalid, value for Custom globalization mode is required when GlobalizationMode=Custom.");

                // predefined ICU name can be identical with the icu files from runtime pack
                expected.Add(Path.GetFileName(assertOptions.BuildOptions.CustomIcuFile));
                break;
            case GlobalizationMode.Sharded:
                // icu shard chosen based on the locale
                expected.Add("icudt_CJK.dat");
                expected.Add("icudt_EFIGS.dat");
                expected.Add("icudt_no_CJK.dat");
                break;
            default:
                throw new NotImplementedException($"Unknown {nameof(assertOptions.BuildOptions.GlobalizationMode)} = {assertOptions.BuildOptions.GlobalizationMode}");
        }

        IEnumerable<string> actual = Directory.EnumerateFiles(assertOptions.BinFrameworkDir, "icudt*dat");

        if (IsFingerprintingEnabled)
        {
            var expectedFingerprinted = new List<string>(expected.Count);
            foreach (var expectedItem in expected)
            {
                var expectedFingerprintedItem = assets.icu.FirstOrDefault(a => a.virtualPath == expectedItem)?.name;
                if (string.IsNullOrEmpty(expectedFingerprintedItem))
                    throw new XunitException($"Could not find ICU asset {expectedItem} in fingerprinting in boot config");

                expectedFingerprinted.Add(expectedFingerprintedItem);
            }

            expected = expectedFingerprinted;
        }

        AssertFileNames(expected, actual);
        if (assertOptions.BuildOptions.GlobalizationMode is GlobalizationMode.Custom)
        {
            string srcPath = assertOptions.BuildOptions.CustomIcuFile!;
            string runtimePackDir = BuildTestBase.s_buildEnv.GetRuntimeNativeDir(assertOptions.BuildOptions.TargetFramework, assertOptions.BuildOptions.RuntimeType);
            if (!Path.IsPathRooted(srcPath))
                srcPath = Path.Combine(runtimePackDir, assertOptions.BuildOptions.CustomIcuFile!);
            TestUtils.AssertSameFile(srcPath, actual.Single());
        }
    }

    public BootJsonData GetBootJson(string bootJsonPath)
    {
        Assert.True(File.Exists(bootJsonPath), $"Expected to find {bootJsonPath}");
        return ParseBootData(bootJsonPath);
    }

    public string GetBootConfigPath(string binFrameworkDir, string? bootConfigFileName = null)
    {
        string[] allDotnetFiles = [
            "dotnet.runtime",
            "dotnet.native",
            "dotnet.native.worker",
            "dotnet.diagnostics"
        ];

        bootConfigFileName ??= "dotnet.js";

        if (bootConfigFileName.EndsWith(".js"))
        {
            string bootFileNameWithoutExtension = Path.GetFileNameWithoutExtension(bootConfigFileName);
            string bootFileExtension = Path.GetExtension(bootConfigFileName);
            string? fingerprintedBootJsonPath = Directory
                .EnumerateFiles(binFrameworkDir)
                .FirstOrDefault(f =>
                {
                    if (Path.GetExtension(f) != bootFileExtension)
                        return false;

                    string fileName = Path.GetFileName(f);
                    if (!fileName.StartsWith(bootFileNameWithoutExtension))
                        return false;
                    
                    if (allDotnetFiles.Except([bootFileNameWithoutExtension]).Any(a => fileName.StartsWith(a)))
                        return false;

                    return true;
                });
            
            if (fingerprintedBootJsonPath == null)
                throw new XunitException($"Could not find boot config '{bootConfigFileName}' with fingerprint in '{binFrameworkDir}'");

            return fingerprintedBootJsonPath;
        }
        else
        {
            return Path.Combine(binFrameworkDir, bootConfigFileName);
        }
    }

    public BootJsonData AssertBootJson(AssertBundleOptions options)
    {

        EnsureProjectDirIsSet();
        string bootJsonPath = GetBootConfigPath(options.BinFrameworkDir, options.BuildOptions.BootConfigFileName);
        BootJsonData bootJson = GetBootJson(bootJsonPath);
        AssetsData assets = (AssetsData)bootJson.resources;

        string spcExpectedFilename = $"System.Private.CoreLib{WasmAssemblyExtension}";

        string? spcActualFilename = assets.coreAssembly.SingleOrDefault(a => a.virtualPath == spcExpectedFilename)?.name;
        if (spcActualFilename is null)
            throw new XunitException($"Could not find an assembly named System.Private.CoreLib.* in {bootJsonPath}");

        var bootJsonEntries = assets.jsModuleNative.Select(a => a.name)
            .Union(assets.wasmNative.Select(a => a.name))
            .Union(assets.jsModuleRuntime.Select(a => a.name))
            .Union(assets.jsModuleWorker?.Select(a => a.name) ?? Enumerable.Empty<string>())
            .Union(assets.jsModuleDiagnostics?.Select(a => a.name) ?? Enumerable.Empty<string>())
            .Union(assets.wasmSymbols?.Select(a => a.name) ?? Enumerable.Empty<string>())
            .ToArray();

        var expectedEntries = new SortedDictionary<string, Func<string, bool>>();
        IReadOnlySet<string> expected = GetDotNetFilesExpectedSet(options);

        var knownSet = GetAllKnownDotnetFilesToFingerprintMap(options);
        foreach (string expectedFilename in expected)
        {
            // FIXME: Find a systematic solution for skipping dotnet.js from boot json check
            if (expectedFilename == "dotnet.js" || Path.GetExtension(expectedFilename) == ".map")
                continue;

            bool expectFingerprint = knownSet[expectedFilename];
            expectedEntries[expectedFilename] = item =>
            {
                string prefix = Path.GetFileNameWithoutExtension(expectedFilename);
                string extension = Path.GetExtension(expectedFilename).Substring(1);

                if (ShouldCheckFingerprint(expectedFilename: expectedFilename,
                                           expectFingerprintOnDotnetJs: options.ExpectDotnetJsFingerprinting,
                                           expectFingerprintForThisFile: expectFingerprint))
                {
                    return Regex.Match(item, $"{prefix}{s_dotnetVersionHashRegex}{extension}").Success;
                }
                else
                {
                    return expectedFilename == item;
                }
            };
        }
        // FIXME: maybe use custom code so the details can show up in the log
        bootJsonEntries = bootJsonEntries.ToArray();
        if (bootJsonEntries.Length != expectedEntries.Count)
        {
            throw new XunitException($"In {bootJsonPath}{Environment.NewLine}" +
                                        $"  Expected: {string.Join(", ", expectedEntries.Keys.ToArray())}{Environment.NewLine}" +
                                        $"  Actual  : {string.Join(", ", bootJsonEntries)}");
        }

        var expectedEntriesToCheck = expectedEntries.Values.ToList();
        foreach (var bootJsonEntry in bootJsonEntries)
        {
            var matcher = expectedEntriesToCheck.FirstOrDefault(c => c(bootJsonEntry));
            if (matcher == null)
                throw new XunitException($"Unexpected entry in boot json '{bootJsonEntry}'. Expected files {String.Join(", ", expectedEntries.Keys)}");

            expectedEntriesToCheck.Remove(matcher);
        }

        return bootJson;
    }

    public static BootJsonData ParseBootData(string bootConfigPath)
    {
        string jsonContent = GetBootJsonContent(bootConfigPath);
        try
        {
            var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
            options.Converters.Add(new ResourcesConverter());
            BootJsonData? config = JsonSerializer.Deserialize<BootJsonData>(jsonContent, options);
            Assert.NotNull(config);
            return config!;
        }
        catch (JsonException e)
        {
            throw new XunitException($"Parsing config failed{Environment.NewLine}{Environment.NewLine}{jsonContent}", e);
        }
    }

    public static string GetBootJsonContent(string bootConfigPath)
    {
        string startComment = "/*json-start*/";
        string endComment = "/*json-end*/";

        string moduleContent = File.ReadAllText(bootConfigPath);
        int startCommentIndex = moduleContent.IndexOf(startComment);
        int endCommentIndex = moduleContent.IndexOf(endComment);
        if (startCommentIndex >= 0 && endCommentIndex >= 0)
        {
            // boot.js
            int startJsonIndex = startCommentIndex + startComment.Length;
            string jsonContent = moduleContent.Substring(startJsonIndex, endCommentIndex - startJsonIndex);
            return jsonContent;
        }

        return moduleContent;
    }

    private void AssertFileNames(IEnumerable<string> expected, IEnumerable<string> actual)
    {
        expected = expected.Order().Select(f => Path.GetFileName(f)).Distinct();
        var actualFileNames = actual.Order().Select(f => Path.GetFileName(f));
        if (expected.Count() != actualFileNames.Count())
        {
            throw new XunitException(
                    $"Expected: {string.Join(", ", expected)}{Environment.NewLine}" +
                    $"Actual:   {string.Join(", ", actualFileNames)}");
        }

        Assert.Equal(expected, actualFileNames);
    }

    public virtual string GetBinFrameworkDir(Configuration config, bool forPublish, string framework, string? projectDir = null)
    {
        throw new NotImplementedException();
    }

    public string GetObjDir(Configuration config, string framework, string? projectDir = null)
    {
        EnsureProjectDirIsSet();
        return Path.Combine(projectDir ?? ProjectDir!, "obj", config.ToString(), framework);
    }

    [MemberNotNull(nameof(ProjectDir))]
    protected void EnsureProjectDirIsSet()
    {
        if (string.IsNullOrEmpty(ProjectDir))
            throw new Exception($"{nameof(ProjectDir)} is not set");
    }

    internal class ResourcesConverter : JsonConverter<object>
    {
        public override object Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            var nestedOptions = new JsonSerializerOptions(options);
            nestedOptions.Converters.Remove(this);

            if (reader.TokenType == JsonTokenType.StartObject)
            {
                try
                {

                    return JsonSerializer.Deserialize<AssetsData>(ref reader, nestedOptions)!;
                }
                catch
                {
                    return JsonSerializer.Deserialize<ResourcesData>(ref reader, nestedOptions)!;
                }
            }

            return JsonSerializer.Deserialize<object>(ref reader, nestedOptions)!;
        }

        public override void Write(Utf8JsonWriter writer, object value, JsonSerializerOptions options)
        {
            JsonSerializer.Serialize(writer, value, value.GetType(), options);
        }
    }
}
