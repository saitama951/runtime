using System;
using System.Runtime.CompilerServices;
using TestLibrary;
using Xunit;

public class Program
{
    [SkipLocalsInit]
    [Fact]
    [ActiveIssue("https://github.com/dotnet/runtime/issues/91923", typeof(PlatformDetection), nameof(PlatformDetection.IsAppleMobile))]
    public static unsafe int TestEntryPoint()
    {
#pragma warning disable CS8500 // takes address of managed type
        bool result = true;

        int poisoned;
        Unsafe.SkipInit(out poisoned);
        result &= VerifyPoison(&poisoned, sizeof(int));

        GCRef zeroed;
        Unsafe.SkipInit(out zeroed);
        result &= VerifyZero(&zeroed, sizeof(GCRef));

        WithoutGCRef poisoned2;
        Unsafe.SkipInit(out poisoned2);
        result &= VerifyPoison(&poisoned2, sizeof(WithoutGCRef));

        Massive poisoned3;
        Unsafe.SkipInit(out poisoned3);
        result &= VerifyPoison(&poisoned3, sizeof(Massive));

        WithoutGCRef poisoned4;
        Unsafe.SkipInit(out poisoned4);
        result &= VerifyPoison(&poisoned4, sizeof(WithoutGCRef));

        Massive poisoned5;
        Unsafe.SkipInit(out poisoned5);
        result &= VerifyPoison(&poisoned5, sizeof(Massive));

        GCRef zeroed2;
        Unsafe.SkipInit(out zeroed2);
        result &= VerifyZero(&zeroed2, sizeof(GCRef));

        return result ? 100 : 101;
#pragma warning restore CS8500
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static unsafe bool VerifyPoison(void* val, int size)
        => AllEq(new Span<byte>(val, size), 0xCD);

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static unsafe bool VerifyZero(void* val, int size)
        => AllEq(new Span<byte>(val, size), 0);

    private static unsafe bool AllEq(Span<byte> span, byte byteVal)
    {
        foreach (byte b in span)
        {
            if (b != byteVal)
                return false;
        }

        return true;
    }

    private struct GCRef
    {
        public object ARef;
    }

    private struct WithoutGCRef
    {
        public double ADouble;
        public int ANumber;
        public float AFloat;
    }

    private unsafe struct Massive
    {
        public fixed byte Bytes[0x10008];
    }
}
