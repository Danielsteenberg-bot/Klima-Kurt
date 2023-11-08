# Use the .NET SDK image to build the application
FROM mcr.microsoft.com/dotnet/sdk:6.0 AS build
WORKDIR /src

# Copy the project file and restore dependencies
COPY ["KnockAPI/KnockAPI.csproj", "KnockAPI/"]
RUN dotnet restore "KnockAPI/KnockAPI.csproj"

# Copy the source code
COPY . .

# Build the application
WORKDIR "/src/KnockAPI"
RUN dotnet build "KnockAPI.csproj" -c Release -o /app/build

# Publish the application
FROM build AS publish
RUN dotnet publish "KnockAPI.csproj" -c Release -o /app/publish /p:UseAppHost=false

# Use the ASP.NET runtime image for the final image
FROM mcr.microsoft.com/dotnet/aspnet:6.0 AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "KnockAPI.dll"]
