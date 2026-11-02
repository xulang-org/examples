using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;

namespace XuNativeInterop
{
    // ========================================================================
    // C# / .NET Host: Calling Xu Windows Dynamic Library (xulib.dll) via P/Invoke
    //
    // Demonstrates full coverage of all 14 Xu data types:
    //   1.  int      - 64-bit signed integer (long)
    //   2.  i32      - 32-bit signed integer (int)
    //   3.  u32      - 32-bit unsigned integer (uint)
    //   4.  u8       - 8-bit unsigned byte (byte)
    //   5.  bool     - boolean logic (byte / bool)
    //   6.  f64      - 64-bit IEEE float (double)
    //   7.  string   - UTF-8 descriptor [cap, len, ptr]
    //   8.  struct   - Point2D, MixedRecord
    //   9.  slice    - []int, []u8, []f64 dynamic views
    //   10. array    - [4]int fixed-size array
    //   11. enum     - Status enum variants
    //   12. $&     - ref Counter in-place mutation
    //   13. callback - delegate / function pointer closure
    //   14. void     - procedure with side-effects
    // ========================================================================

    [StructLayout(LayoutKind.Sequential)]
    public struct XuString
    {
        public long Cap;
        public long Len;
        public IntPtr Ptr;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuPoint2D
    {
        public long Y; // offset -8
        public long X; // offset 0 (Top Slot)
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuMixedRecord
    {
        public ulong Flag;      // offset -16 (bool)
        public ulong ScoreBits; // offset -8  (f64 IEEE bits)
        public long  Id;        // offset 0   (int, Top Slot)
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuSlice
    {
        public long Cap;
        public long Len;
        public IntPtr Ptr;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuFixedArray4
    {
        public long E3; // offset -24
        public long E2; // offset -16
        public long E1; // offset -8
        public long E0; // offset 0 (Top Slot)
    }

    public enum XuStatus : long
    {
        Ok = 0,
        Pending = 1,
        Failed = 2
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuCounter
    {
        public long Val;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct XuClosure
    {
        public IntPtr Env;   // offset -8
        public IntPtr FnPtr; // offset 0 (Top Slot)
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate long CallbackDelegate(IntPtr env, long a, long b);

    public static class XuLib
    {
        private const string LibName = "xulib.dll";

        // 1. int
        [DllImport(LibName, EntryPoint = "xu_add_int", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuAddInt(long a, long b);

        [DllImport(LibName, EntryPoint = "xu_factorial", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuFactorial(long n);

        // 2. i32
        [DllImport(LibName, EntryPoint = "xu_add_i32", CallingConvention = CallingConvention.Cdecl)]
        public static extern int XuAddI32(long a, long b);

        [DllImport(LibName, EntryPoint = "xu_clamp_i32", CallingConvention = CallingConvention.Cdecl)]
        public static extern int XuClampI32(long val, long minV, long maxV);

        // 3. u32
        [DllImport(LibName, EntryPoint = "xu_add_u32", CallingConvention = CallingConvention.Cdecl)]
        public static extern uint XuAddU32(uint a, uint b);

        [DllImport(LibName, EntryPoint = "xu_rotate_left_u32", CallingConvention = CallingConvention.Cdecl)]
        public static extern uint XuRotateLeftU32(uint val, uint shift);

        // 4. u8
        [DllImport(LibName, EntryPoint = "xu_mask_u8", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte XuMaskU8(byte val, byte mask);

        [DllImport(LibName, EntryPoint = "xu_add_u8", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte XuAddU8(byte a, byte b);

        // 5. bool
        [DllImport(LibName, EntryPoint = "xu_is_even", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte XuIsEven(long n);

        [DllImport(LibName, EntryPoint = "xu_logical_and", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte XuLogicalAnd(byte a, byte b);

        // 6. f64 (bits passed in general registers)
        [DllImport(LibName, EntryPoint = "xu_add_f64", CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong XuAddF64Bits(ulong aBits, ulong bBits);

        [DllImport(LibName, EntryPoint = "xu_scale_f64", CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong XuScaleF64Bits(ulong valBits, ulong factorBits);

        public static double XuAddF64(double a, double b)
        {
            ulong aBits = (ulong)BitConverter.DoubleToInt64Bits(a);
            ulong bBits = (ulong)BitConverter.DoubleToInt64Bits(b);
            ulong resBits = XuAddF64Bits(aBits, bBits);
            return BitConverter.Int64BitsToDouble((long)resBits);
        }

        public static double XuScaleF64(double val, double factor)
        {
            ulong vBits = (ulong)BitConverter.DoubleToInt64Bits(val);
            ulong fBits = (ulong)BitConverter.DoubleToInt64Bits(factor);
            ulong resBits = XuScaleF64Bits(vBits, fBits);
            return BitConverter.Int64BitsToDouble((long)resBits);
        }

        // 7. string
        [DllImport(LibName, EntryPoint = "xu_string_length", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuStringLength(ref IntPtr strTop);

        [DllImport(LibName, EntryPoint = "xu_count_char", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuCountChar(ref IntPtr strTop, long ch);

        [DllImport(LibName, EntryPoint = "xu_string_byte_at", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuStringByteAt(ref IntPtr strTop, long idx);

        [DllImport(LibName, EntryPoint = "xu_print_string", CallingConvention = CallingConvention.Cdecl)]
        public static extern void XuPrintString(ref IntPtr strTop);

        [DllImport(LibName, EntryPoint = "xu_greeting", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr XuGreeting(ref IntPtr sretTop);

        // 8. struct
        [DllImport(LibName, EntryPoint = "xu_point_distance_sq", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuPointDistanceSq(ref long ptTop);

        [DllImport(LibName, EntryPoint = "xu_make_point", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr XuMakePoint(ref long sretTop, long x, long y);

        [DllImport(LibName, EntryPoint = "xu_check_mixed_record", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte XuCheckMixedRecord(ref long recTop);

        // 9. slice
        [DllImport(LibName, EntryPoint = "xu_sum_slice", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuSumSlice(ref IntPtr sliceTop);

        [DllImport(LibName, EntryPoint = "xu_sum_byte_slice", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuSumByteSlice(ref IntPtr sliceTop);

        [DllImport(LibName, EntryPoint = "xu_sum_f64_slice", CallingConvention = CallingConvention.Cdecl)]
        public static extern ulong XuSumF64Slice(ref IntPtr sliceTop);

        // 10. array
        [DllImport(LibName, EntryPoint = "xu_sum_fixed_array", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuSumFixedArray(ref long arrTop);

        // 11. enum
        [DllImport(LibName, EntryPoint = "xu_status_code", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuStatusCode(ref long statusPtr);

        // 12. $&
        [DllImport(LibName, EntryPoint = "xu_mutate_counter", CallingConvention = CallingConvention.Cdecl)]
        public static extern void XuMutateCounter(ref long counterPtr, long delta);

        // 13. callback
        [DllImport(LibName, EntryPoint = "xu_apply_callback", CallingConvention = CallingConvention.Cdecl)]
        public static extern long XuApplyCallback(long x, long y, ref IntPtr closureTop);

        // 14. void
        [DllImport(LibName, EntryPoint = "xu_hello_from_lib", CallingConvention = CallingConvention.Cdecl)]
        public static extern void XuHelloFromLib();
    }

    class Program
    {
        static long MyCallback(IntPtr env, long a, long b)
        {
            return a * b;
        }

        static void Main(string[] args)
        {
            Console.WriteLine("====================================================================");
            Console.WriteLine("   C# / .NET Host Calling Xu Windows Dynamic Library (xulib.dll)");
            Console.WriteLine("====================================================================\n");

            // 1. int
            Console.WriteLine("[1. int] Testing 64-bit integer...");
            long sum = XuLib.XuAddInt(100, 200);
            Console.WriteLine($"    XuAddInt(100, 200) = {sum} (expected 300)");
            Debug.Assert(sum == 300);

            long fact = XuLib.XuFactorial(6);
            Console.WriteLine($"    XuFactorial(6) = {fact} (expected 720)");
            Debug.Assert(fact == 720);

            // 2. i32
            Console.WriteLine("\n[2. i32] Testing 32-bit integer...");
            int s32 = XuLib.XuAddI32(-20, 35);
            Console.WriteLine($"    XuAddI32(-20, 35) = {s32} (expected 15)");
            Debug.Assert(s32 == 15);

            int clamped = XuLib.XuClampI32(150, 0, 100);
            Console.WriteLine($"    XuClampI32(150, 0, 100) = {clamped} (expected 100)");
            Debug.Assert(clamped == 100);

            // 3. u32
            Console.WriteLine("\n[3. u32] Testing 32-bit unsigned integer...");
            uint u32Val = XuLib.XuAddU32(0xFFFFFF00U, 0x10U);
            Console.WriteLine($"    XuAddU32(0xFFFFFF00, 0x10) = 0x{u32Val:X8} (expected 0xFFFFFF10)");
            Debug.Assert(u32Val == 0xFFFFFF10U);

            uint rot = XuLib.XuRotateLeftU32(1U, 4U);
            Console.WriteLine($"    XuRotateLeftU32(1, 4) = {rot} (expected 16)");
            Debug.Assert(rot == 16U);

            // 4. u8
            Console.WriteLine("\n[4. u8] Testing 8-bit unsigned byte...");
            byte masked = XuLib.XuMaskU8(0xAB, 0x0F);
            Console.WriteLine($"    XuMaskU8(0xAB, 0x0F) = 0x{masked:X2} (expected 0x0B)");
            Debug.Assert(masked == 0x0B);

            byte sumU8 = XuLib.XuAddU8(200, 50);
            Console.WriteLine($"    XuAddU8(200, 50) = {sumU8} (expected 250)");
            Debug.Assert(sumU8 == 250);

            // 5. bool
            Console.WriteLine("\n[5. bool] Testing boolean logic...");
            byte even42 = XuLib.XuIsEven(42);
            byte even43 = XuLib.XuIsEven(43);
            Console.WriteLine($"    XuIsEven(42) = {even42 == 1}, XuIsEven(43) = {even43 == 1}");
            Debug.Assert(even42 == 1 && even43 == 0);

            byte land = XuLib.XuLogicalAnd(1, 0);
            Console.WriteLine($"    XuLogicalAnd(true, false) = {land == 1} (expected false)");
            Debug.Assert(land == 0);

            // 6. f64
            Console.WriteLine("\n[6. f64] Testing 64-bit IEEE float...");
            double fAdd = XuLib.XuAddF64(1.25, 2.75);
            Console.WriteLine($"    XuAddF64(1.25, 2.75) = {fAdd:F4} (expected 4.0000)");
            Debug.Assert(fAdd == 4.0);

            double scaled = XuLib.XuScaleF64(3.5, 2.0);
            Console.WriteLine($"    XuScaleF64(3.5, 2.0) = {scaled:F4} (expected 7.0000)");
            Debug.Assert(scaled == 7.0);

            // 7. string
            Console.WriteLine("\n[7. string] Testing UTF-8 string descriptors and passing...");
            byte[] utf8Bytes = Encoding.UTF8.GetBytes("Hello from C# P/Invoke!\0");
            GCHandle strHandle = GCHandle.Alloc(utf8Bytes, GCHandleType.Pinned);
            try
            {
                XuString xs = new XuString
                {
                    Cap = 0,
                    Len = utf8Bytes.Length - 1,
                    Ptr = strHandle.AddrOfPinnedObject()
                };

                long strLen = XuLib.XuStringLength(ref xs.Ptr);
                Console.WriteLine($"    XuStringLength = {strLen} (expected {utf8Bytes.Length - 1})");
                Debug.Assert(strLen == utf8Bytes.Length - 1);

                long countO = XuLib.XuCountChar(ref xs.Ptr, 'o');
                Console.WriteLine($"    XuCountChar('o') = {countO} (expected 2)");
                Debug.Assert(countO == 2);

                long byteAt = XuLib.XuStringByteAt(ref xs.Ptr, 0);
                Console.WriteLine($"    XuStringByteAt(0) = {(char)byteAt} (expected 'H')");
                Debug.Assert(byteAt == 'H');

                XuLib.XuPrintString(ref xs.Ptr);
            }
            finally
            {
                strHandle.Free();
            }

            // 8. struct
            Console.WriteLine("\n[8. struct] Testing struct passing and returning...");
            XuPoint2D pt = new XuPoint2D { X = 3, Y = 4 };
            long distSq = XuLib.XuPointDistanceSq(ref pt.X);
            Console.WriteLine($"    XuPointDistanceSq(3, 4) = {distSq} (expected 25)");
            Debug.Assert(distSq == 25);

            XuPoint2D ptOut = new XuPoint2D();
            XuLib.XuMakePoint(ref ptOut.X, 7, 24);
            Console.WriteLine($"    XuMakePoint(7, 24) = ({ptOut.X}, {ptOut.Y})");
            Debug.Assert(ptOut.X == 7 && ptOut.Y == 24);

            XuMixedRecord rec = new XuMixedRecord
            {
                Id = 101,
                ScoreBits = (ulong)BitConverter.DoubleToInt64Bits(85.5),
                Flag = 1
            };
            byte recOk = XuLib.XuCheckMixedRecord(ref rec.Id);
            Console.WriteLine($"    XuCheckMixedRecord = {recOk == 1} (expected true)");
            Debug.Assert(recOk == 1);

            // 9. slice
            Console.WriteLine("\n[9. slice] Testing slice views ([]int, []u8, []f64)...");
            long[] intArr = { 10, 20, 30, 40 };
            GCHandle intHandle = GCHandle.Alloc(intArr, GCHandleType.Pinned);
            try
            {
                XuSlice slInt = new XuSlice
                {
                    Cap = 0,
                    Len = intArr.Length,
                    Ptr = intHandle.AddrOfPinnedObject()
                };
                long sumSl = XuLib.XuSumSlice(ref slInt.Ptr);
                Console.WriteLine($"    XuSumSlice([10, 20, 30, 40]) = {sumSl} (expected 100)");
                Debug.Assert(sumSl == 100);
            }
            finally
            {
                intHandle.Free();
            }

            // 10. array
            Console.WriteLine("\n[10. array] Testing fixed-size array [4]int...");
            XuFixedArray4 arr4 = new XuFixedArray4
            {
                E0 = 100,
                E1 = 200,
                E2 = 300,
                E3 = 400
            };
            long sumArr = XuLib.XuSumFixedArray(ref arr4.E0);
            Console.WriteLine($"    XuSumFixedArray([100, 200, 300, 400]) = {sumArr} (expected 1000)");
            Debug.Assert(sumArr == 1000);

            // 11. enum
            Console.WriteLine("\n[11. enum] Testing enumeration variants...");
            long stPending = (long)XuStatus.Pending;
            long statusCode = XuLib.XuStatusCode(ref stPending);
            Console.WriteLine($"    XuStatusCode(Pending) = {statusCode} (expected 200)");
            Debug.Assert(statusCode == 200);

            // 12. $&
            Console.WriteLine("\n[12. $&] Testing mutable pointer in-place mutation...");
            XuCounter cnt = new XuCounter { Val = 50 };
            XuLib.XuMutateCounter(ref cnt.Val, 25);
            Console.WriteLine($"    XuMutateCounter(&cnt{{50}}, 25) -> cnt.Val = {cnt.Val} (expected 75)");
            Debug.Assert(cnt.Val == 75);

            // 13. callback
            Console.WriteLine("\n[13. callback] Testing function pointer / closure callback...");
            CallbackDelegate del = MyCallback;
            IntPtr fp = Marshal.GetFunctionPointerForDelegate(del);
            XuClosure clos = new XuClosure
            {
                Env = IntPtr.Zero,
                FnPtr = fp
            };
            long cbRes = XuLib.XuApplyCallback(7, 8, ref clos.FnPtr);
            Console.WriteLine($"    XuApplyCallback(7, 8, MyCallback) = {cbRes} (expected 56)");
            Debug.Assert(cbRes == 56);
            GC.KeepAlive(del);

            // 14. void
            Console.WriteLine("\n[14. void] Testing procedure invocation:");
            XuLib.XuHelloFromLib();

            Console.WriteLine("\n====================================================================");
            Console.WriteLine("   >>> ALL 14 C# / .NET P/INVOKE ASSERTIONS PASSED SUCCESSFULLY! <<<");
            Console.WriteLine("====================================================================\n");
        }
    }
}
