using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HiveMQ.Models
{
    public class Event : Common
    {
        public string Sensor { get; set; }

        public int Data { get; set; }
    }
}
