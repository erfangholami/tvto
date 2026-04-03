using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace exomni
{
    class fkik
    {
        static float r = 0.076f; //radius
        static float l = 0.30f;

        public static int Convert_to_byte(float x)
        {
            float rpm = (x * 60 / (2 * (float)Math.PI));
            return (int)(rpm * ( (float)128 / 122 ));
        }

        public static float Convert_to_value(float x)
        {
            float y = x * ((float)122 / 128);

            float radps = (y * (2 * (float)Math.PI) / 60);
            return radps;
        }

        public static void _IK()
        {
            form.write1 = Convert_to_byte(13.15f * form.in_VX + 13.15f * form.in_VY + 18.673f * form.in_VW * 3.14f / 180);
            form.write2 = Convert_to_byte(13.15f * form.in_VX - 13.15f * form.in_VY - 18.673f * form.in_VW * 3.14f / 180);
            form.write3 = Convert_to_byte(13.15f * form.in_VX + 13.15f * form.in_VY - 18.673f * form.in_VW * 3.14f / 180);
            form.write4 = Convert_to_byte(13.15f * form.in_VX - 13.15f * form.in_VY + 18.673f * form.in_VW * 3.14f / 180);
        }

        public static void _FK()
        {
           form.out_VX = (int)((0.019f * (Convert_to_value(form.in_Motor1) + Convert_to_value(form.in_Motor2) + Convert_to_value(form.in_Motor3) + Convert_to_value(form.in_Motor4))) * 100);
           form.out_VY = (int)((0.019f * (Convert_to_value(form.in_Motor1) - Convert_to_value(form.in_Motor2) + Convert_to_value(form.in_Motor3) - Convert_to_value(form.in_Motor4))) * 100);
           form.out_VW = ((0.013f * (Convert_to_value(form.in_Motor1) - Convert_to_value(form.in_Motor2) - Convert_to_value(form.in_Motor3) + Convert_to_value(form.in_Motor4))) * 180 / 3.14f);
        }
    }
}
