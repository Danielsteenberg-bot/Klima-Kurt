using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using Microsoft.EntityFrameworkCore;
using KnockModels;



namespace KnockAPI.Data
{
    public class DataContext : DbContext
    {
        public DataContext(DbContextOptions<DataContext> options) : base(options) { }

        public DbSet<User> Users { get; set; }

        public DbSet<Code> Codes { get; set; }

        public DbSet<Door> Doors { get; set; }

        public DbSet<Event> Events { get; set; }

    }
}
