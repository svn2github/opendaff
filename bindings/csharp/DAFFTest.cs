using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DAFF;

namespace DAFF
{
    class DAFFTest
    {
        /// <summary>
        /// Console application that demonstrates how to load and access DAFF content in C#.
        /// </summary>
        /// <param name="args">File path</param>
        static void Main(string[] args)
        {
            string FilePath;
            if (args.Length == 0)
                FilePath = "ITA-Kunstkopf_HRIR_AP11_Pressure_Equalized_3x3_256.v17.ir.daff";
            else
                FilePath = args[0];
            
            Console.WriteLine("Attempting to open DAFF file from path " + FilePath);

            DAFFReader MyDAFFReader = new DAFFReader();
            if( !MyDAFFReader.Load(FilePath) )
                Console.WriteLine( "Could not load DAFF file from path " + FilePath);

            Console.WriteLine("DAFF content type is " + MyDAFFReader.GetContentType());

            if (MyDAFFReader.GetContentType() == ContentType.ImpulseResponse)
            {
                IR HRIR = MyDAFFReader.GetContentIR();
                int RecordIndex = HRIR.GetNearestNeighbourRecordIndex(-90, 13);
                float[] HRIR_L;
                float[] HRIR_R;
                HRIR.GetRecordData(RecordIndex, 0, out HRIR_L);
                HRIR.GetRecordData(RecordIndex, 1, out HRIR_R);
            }
            else if(MyDAFFReader.GetContentType() == ContentType.MagnitudeSpectrum)
            {
                MS Directivity = MyDAFFReader.GetContentMS();
                int RecordIndex = Directivity.GetNearestNeighbourRecordIndex(45, -11);
                float[] DirectivitySpectrum;
                Directivity.GetRecordData(RecordIndex, 0, out DirectivitySpectrum);
            }
            else
            {
                Console.WriteLine("DAFF content type is unrecognized and not supported by C# binding");
            }

            return;
        }
    }
}
