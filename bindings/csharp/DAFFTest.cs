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
        static void Main(string[] args)
        {
            DAFFReader MyDAFFReader = new DAFFReader();
            string FilePath = "ITA-Kunstkopf_HRIR_AP11_Pressure_Equalized_3x3_256.v17.ir.daff";
            Console.WriteLine("Opening file: " + FilePath);
            MyDAFFReader.Load(FilePath);

            Console.WriteLine("Content type id: " + MyDAFFReader.GetContentType());

            if (MyDAFFReader.GetContentType() == 0)
            {
                IR HRIR = MyDAFFReader.GetContentIR();
                int RecordIndex = HRIR.GetNearestNeighbourRecordIndex(-90, 13);
                float[] HRIR_L;
                float[] HRIR_R;
                HRIR.GetRecordData(RecordIndex, 0, out HRIR_L);
                HRIR.GetRecordData(RecordIndex, 1, out HRIR_R);
            }
            else if(MyDAFFReader.GetContentType() == 1)
            {
                MS Directivity = MyDAFFReader.GetContentMS();
                int RecordIndex = Directivity.GetNearestNeighbourRecordIndex(45, -11);
                float[] DirectivitySpectrum;
                Directivity.GetRecordData(RecordIndex, 0, out DirectivitySpectrum);
            }
        }
    }
}
