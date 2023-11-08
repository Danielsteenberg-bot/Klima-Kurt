using Microsoft.EntityFrameworkCore;
using KnockModels;

namespace KnockMQTT
{
    public class MqttDbContext : DbContext
    {
        public DbSet<Event> Events { get; set; }
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer("Data Source=knockknockshield3k.database.windows.net,1433;Database=knockDB;User ID=admingoat;Password=58246Poulsen;Connect Timeout=30;Encrypt=True;Trust Server Certificate=False;Application Intent=ReadWrite;Multi Subnet Failover=False");
        }
    }
}
