using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Collections.Generic;

/// <summary>
///  OpenDAFF - A free, open source software package for directional audio data
///  Copyright 2016 - 2018 Institute of Technical Acoustics, RWTH Aachen University
///  OpenDAFF is distributed under the Apache License Version 2.0.
///  More information and directional content can be found at http://www.opendaff.org.
/// </summary>
namespace DAFF
{
    /// <summary>
    /// Definition of available content types
    /// </summary>
    public enum ContentType
    {
        ImpulseResponse,
        MagnitudeSpectrum,
        Invalid,
    }

    /// <summary>
    ///  The DAFFReader class is used to load a DAFF file and access the fundamental properties such as
    ///  the type of content. Depending on the type, the actual content can be requested and accessd by
    ///  individual classes such as IR and MS.
    /// </summary>
    public class DAFFReader
    {
        private IntPtr _DAFFHandle;
        
        /// <summary>
        /// Create a DAFF reader that can load data and provides access to different content types.
        /// </summary>
        public DAFFReader()
        {
            _DAFFHandle = NativeDAFFCreate();
        }

        ~DAFFReader()
        {
            NativeDAFFDispose(_DAFFHandle);
        }

        /// <summary>
        /// Load a DAFF file from given path.
        /// </summary>
        /// <param name="FilePath">Path to DAFF file</param>
        /// <returns>True, if file could be loaded, false otherwise.</returns>
        public bool Load(string FilePath)
        {
            return NativeDAFFLoad(_DAFFHandle, FilePath);
        }

        /// <summary>
        /// Indivator if a file has been loaded and is ready to use.
        /// Make sure that your content is valid and accessible in your code before calling further methods.
        /// </summary>
        /// <returns>True, if data present and ready to use</returns>
        public bool IsDataPresent()
        {
            return (_DAFFHandle != null);
        }

        /// <summary>
        /// Returns content type as integer, see also ContentType enumeration.
        /// </summary>
        /// <returns>Content type as integer</returns>
        public ContentType GetContentType()
        {
            int NativeContentType = NativeDAFFGetContentType(_DAFFHandle);
            if (NativeContentType == 0)
                return ContentType.ImpulseResponse;
            if (NativeContentType == 1)
                return ContentType.MagnitudeSpectrum;
            else
                return ContentType.Invalid;
        }

        /// <summary>
        /// Returns content of DAFF file as impulse response time domain data (for HRIRs etc. ... and only if content type matches)
        /// </summary>
        /// <returns>IR content type</returns>
        public IR GetContentIR()
        {
            return new IR(_DAFFHandle); ;
        }

        /// <summary>
        /// Returns content of DAFF file as magnitude spectrum data (for directivities of musical instruments, human speakers, singers etc. ... and only if content type matches)
        /// </summary>
        /// <returns>MS content type</returns>
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

    /// <summary>
    /// Class describing time domain impulse responses.
    /// </summary>
    public class IR
    {
        private IntPtr _DAFFIRHandle;

        /// <summary>
        /// Creates a IR class from the DAFF content passed by DAFFReader. For internal use, see GetContentIR() instead.
        /// </summary>
        /// <param name="_DAFFHandle">Internal DAFF handle</param>
        public IR(IntPtr _DAFFHandle)
        {
            _DAFFIRHandle = NativeDAFFGetContentIR(_DAFFHandle);
        }

        ~IR()
        {

        }

        /// <summary>
        /// Checks if class is valid and ready to use.
        /// </summary>
        /// <returns>True, if ready to use.</returns>
        public bool IsValid()
        {
            return (_DAFFIRHandle != null);
        }

        /// <summary>
        /// Actual accessor for directional DAFF content. Searches for nearest neighbour in data grid. Uses degrees and solely the DAFF object (user) view.
        /// (0,0) is front etc, see DAFF C++ documentation for more details.
        /// </summary>
        /// <param name="Azimuth">Azimuthal angle in degree</param>
        /// <param name="Elevation">Elevation angle in degree</param>
        /// <returns>Index of nearest neighbour record</returns>
        public int GetNearestNeighbourRecordIndex(double Azimuth, double Elevation)
        {
            return NativeDAFFContentIRGetNearestNeighbourRecordIndex(_DAFFIRHandle, Azimuth, Elevation);
        }

        /// <summary>
        /// Retrieves the coordinates in azimuth and elevation (degree) for a record index.
        /// </summary>
        /// <param name="Index">Record index</param>
        /// <param name="Azimuth">Azimuth angle in degree</param>
        /// <param name="Elevation">Elevation angle in degree</param>
        public void GetRecordCoords(int Index, ref double Azimuth, ref double Elevation)
        {
            NativeDAFFContentIRGetRecordCoords(_DAFFIRHandle, Index, ref Azimuth, ref Elevation);
        }

        /// <summary>
        /// Returns the length of the underlying content/data values for one record/dataset.
        /// </summary>
        /// <returns>Number of content values</returns>
        public int GetLength()
        {
            return NativeDAFFContentIRGetLength(_DAFFIRHandle);
        }

        /// <summary>
        /// Retrieves a copy of a record data in given direction by record index, see GetNearestNeighbourRecordIndex.
        /// </summary>
        /// <param name="Index">Record index</param>
        /// <param name="Channel">Channel index (start with 0)</param>
        /// <returns>Array of content values</returns>
        public float[] GetRecordData(int RecordIndex, int ChannelIndex)
        {
            int NumSamples = GetLength();
			float[] Samples = new float[NumSamples];
            if (!NativeDAFFContentIRGetRecordData(_DAFFIRHandle, RecordIndex, ChannelIndex, Samples))
                return null;
			return Samples;
        }

        [DllImport("DAFFCSWrapper")]
        private static extern IntPtr NativeDAFFGetContentIR(IntPtr pHandle);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetNearestNeighbourRecordIndex(IntPtr pHandle, double dAzimuth, double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetRecordCoords(IntPtr pHandle, int iIndex, ref double dAzimuth, ref double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern bool NativeDAFFContentIRGetRecordData(IntPtr pHandle, int iReordIndex, int iChannelIndex, [Out] float[] pfSamples);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentIRGetLength(IntPtr pHandle);

    }
    public class MS
    {
        private IntPtr _DAFFMSHandle;

        /// <summary>
        /// Creates an MS class from the DAFF content passed by DAFFReader. For internal use, see GetContentMS() instead.
        /// </summary>
        /// <param name="_DAFFHandle">Internal DAFF handle</param>
        public MS(IntPtr _DAFFHandle)
        {
            _DAFFMSHandle = NativeDAFFGetContentMS(_DAFFHandle);
        }

        ~MS()
        {

        }

        /// <summary>
        /// Checks if class is valid and ready to use.
        /// </summary>
        /// <returns>True, if ready to use.</returns>
        public bool IsValid()
        {
            return (_DAFFMSHandle != null);
        }

        /// <summary>
        /// Actual accessor for directional DAFF content. Searches for nearest neighbour in data grid. Uses degrees and solely the DAFF object (user) view.
        /// (0,0) is front etc, see DAFF C++ documentation for more details.
        /// </summary>
        /// <param name="Azimuth">Azimuthal angle in degree</param>
        /// <param name="Elevation">Elevation angle in degree</param>
        /// <returns>Index of nearest neighbour record</returns>
        public int GetNearestNeighbourRecordIndex(double Azimuth, double Elevation)
        {
            return NativeDAFFContentMSGetNearestNeighbourRecordIndex(_DAFFMSHandle, Azimuth, Elevation);
        }

        /// <summary>
        /// Retrieves the coordinates in azimuth and elevation (degree) for a record index.
        /// </summary>
        /// <param name="Index">Record index</param>
        /// <param name="Azimuth">Azimuth angle in degree</param>
        /// <param name="Elevation">Elevation angle in degree</param>
        public void GetRecordCoords(int Index, ref double Azimuth, ref double Elevation)
        {
            NativeDAFFContentMSGetRecordCoords(_DAFFMSHandle, Index, ref Azimuth, ref Elevation);
        }

        /// <summary>
        /// Returns the length of the underlying content/data values for one record/dataset.
        /// </summary>
        /// <returns>Number of content values</returns>
        public int GetLength()
        {
            return NativeDAFFContentMSGetLength(_DAFFMSHandle);
        }

        /// <summary>
        /// Retrieves a copy of a record data in given direction by record index, see GetNearestNeighbourRecordIndex.
        /// </summary>
        /// <param name="Index">Record index</param>
        /// <param name="Channel">Channel index (start with 0)</param>
        /// <param name="Samples">Array of content values</param>
        public float[] GetRecordData(int RecordIndex, int ChannelIndex)
        {
            int NumMags = GetLength();
            float[] Magnitudes = new float[NumMags];
            if (!NativeDAFFContentMSGetRecordData(_DAFFMSHandle, RecordIndex, ChannelIndex, Magnitudes))
                return null;
			return Magnitudes;
        }

        [DllImport("DAFFCSWrapper")]
        private static extern IntPtr NativeDAFFGetContentMS(IntPtr pHandle);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetNearestNeighbourRecordIndex(IntPtr pHandle, double dAzimuth, double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetRecordCoords(IntPtr pHandle, int iIndex, ref double dAzimuth, ref double dElevation);

        [DllImport("DAFFCSWrapper")]
        private static extern bool NativeDAFFContentMSGetRecordData(IntPtr pHandle, int iIndex, int iChannelIndex, [Out] float[] fMags);

        [DllImport("DAFFCSWrapper")]
        private static extern int NativeDAFFContentMSGetLength(IntPtr pHandle);

    }
}
