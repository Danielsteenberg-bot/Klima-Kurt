using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace HiveMQ.Migrations
{
    /// <inheritdoc />
    public partial class Virk : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "DeviceDataString",
                table: "DeviceData");

            migrationBuilder.DropColumn(
                name: "DeviceName",
                table: "DeviceData");

            migrationBuilder.AddColumn<string>(
                name: "Sensor",
                table: "DeviceData",
                type: "TEXT",
                nullable: false,
                defaultValue: "");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Sensor",
                table: "DeviceData");

            migrationBuilder.AddColumn<string>(
                name: "DeviceDataString",
                table: "DeviceData",
                type: "TEXT",
                nullable: true);

            migrationBuilder.AddColumn<string>(
                name: "DeviceName",
                table: "DeviceData",
                type: "TEXT",
                nullable: true);
        }
    }
}
