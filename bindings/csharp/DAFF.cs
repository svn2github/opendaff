using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections.Generic;

namespace DAFF
{
    public class DAFFReader
    {
        private IntPtr _DAFFHandle;

        public DAFFReader()
        {
            _DAFFHandle = NativeDAFFCreate();
        }

        ~DAFFReader()
        {
            NativeDAFFDispose(_DAFFHandle);
        }

        public bool Load(string FilePath)
        {
            return NativeDAFFLoad(_DAFFHandle, FilePath);
        }

        public bool IsDataPresent()
        {
            return (_DAFFHandle != null);
        }

        public int GetContentType()
        {
            return NativeDAFFGetContentType(_DAFFHandle);
        }

        public IR GetContentIR()
        {
            return new IR(_DAFFHandle); ;
        }

        public MS GetContentMS()
        {
            return new MS(_DAFFHandle); ;
        }

        [DllImport("DAFFCSWrapper")]
        private static extern IntPtr NativeDAFFCreate();

        [DllImport("DAFFCSWrapper")]
        private static extern void NativeDAFFDispose(IntPtr pHandle);

        [DllImport("DAFFCSWrapper")]
        private static extern bool NativeDAFFLoad(IntPtr pHandle, string sFilePath);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFGetContentType(IntPtr pHandle);
    }


    public class IR
    {
        private IntPtr _DAFFIRHandle;

        public IR(IntPtr _DAFFHandle)
        {
            _DAFFIRHandle = NativeDAFFGetContentIR(_DAFFHandle);
        }

        ~IR()
        {

        }

        public bool IsValid()
        {
            return (_DAFFIRHandle != null);
        }

        public int GetNearestNeighbourRecordIndex(double Azimuth, double Elevation)
        {
            return NativeDAFFContentIRGetRecordIndex(_DAFFIRHandle, Azimuth, Elevation);
        }

        public void GetRecordCoords(int Index, ref double Azimuth, ref double Elevation)
        {
            NativeDAFFContentIRGetRecordCoords(_DAFFIRHandle, Index, ref Azimuth, ref Elevation);
        }

        public void GetRecordData(int Index, int Channel, out float[] Samples)
        {
            int NumSamples = NativeDAFFContentIRGetLength(_DAFFIRHandle);
            Samples = new float[NumSamples];
            NativeDAFFContentIRGetRecordData(_DAFFIRHandle, Index, Channel, out Samples);
        }

        [DllImport("DAFFCSWrapper")]
        private static extern IntPtr NativeDAFFGetContentIR(IntPtr pHandle);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetRecordIndex(IntPtr pHandle, double dAzimuth, double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetRecordCoords(IntPtr pHandle, int iIndex, ref double dAzimuth, ref double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern void NativeDAFFContentIRGetRecordData(IntPtr pHandle, int iIndex, int iChannel, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] out float[] pfSamples);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetLength(IntPtr pHandle);

    }
    public class MS
    {
        private IntPtr _DAFFMSHandle;

        public MS(IntPtr _DAFFHandle)
        {
            _DAFFMSHandle = NativeDAFFGetContentMS(_DAFFHandle);
        }

        ~MS()
        {

        }

        public bool IsValid()
        {
            return (_DAFFMSHandle != null);
        }

        public int GetNearestNeighbourRecordIndex(double Azimuth, double Elevation)
        {
            return NativeDAFFContentMSGetRecordIndex(_DAFFMSHandle, Azimuth, Elevation);
        }

        public void GetRecordCoords(int Index, ref double Azimuth, ref double Elevation)
        {
            NativeDAFFContentMSGetRecordCoords(_DAFFMSHandle, Index, ref Azimuth, ref Elevation);
        }

        public void GetRecordData(int Index, int Channel, out float[] Magnitudes)
        {
            int NumSamples = NativeDAFFContentMSGetLength(_DAFFMSHandle);
            Magnitudes = new float[NumSamples];
            NativeDAFFContentMSGetRecordData(_DAFFMSHandle, Index, Channel, out Magnitudes);
        }

        [DllImport("DAFFCSWrapper")]
        private static extern IntPtr NativeDAFFGetContentMS(IntPtr pHandle);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetRecordIndex(IntPtr pHandle, double dAzimuth, double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetRecordCoords(IntPtr pHandle, int iIndex, ref double dAzimuth, ref double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern void NativeDAFFContentMSGetRecordData(IntPtr pHandle, int iIndex, int iChannel, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] out float[] fMags);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetLength(IntPtr pHandle);

    }
}
