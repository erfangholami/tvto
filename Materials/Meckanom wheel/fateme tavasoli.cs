using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace exomni
{
    class fateme_tavasoli
    {
        public static void _IK()
        {
            form.write1 = (int)(131.96 * (((-6.58) * form.in_VX) + (6.58 * form.in_VY) + 0.18 * (-282.63 * form.in_VW )));
            form.write2 = (int)(131.96 * (((-6.58) * form.in_VX) + ((-6.58) * form.in_VY) + 0.18 * (6.51 * form.in_VW)));
            form.write3 = (int)(131.96 * ((6.58 * form.in_VX) + ((-6.58) * form.in_VY) + 0.18 * (282.63 * form.in_VW)));
            form.write4 = (int)(131.96 * ((6.58 * form.in_VX) + (6.58 * form.in_VY) + 0.18 * (-6.51 * form.in_VW)));
        }

        public static void _FK()
        {
           form.out_VX = 0.008f * 0.038f * (form.in_Motor1 + form.in_Motor2 + form.in_Motor3 + form.in_Motor4);
           form.out_VY = 0.008f * 0.038f * (-form.in_Motor1 + form.in_Motor2 - form.in_Motor3 + form.in_Motor4);
           form.out_VW = 5.72f * (10.74f * (-form.in_Motor1 + form.in_Motor3)) + (0.24f * (form.in_Motor2 - form.in_Motor4));
        }
    }
}
