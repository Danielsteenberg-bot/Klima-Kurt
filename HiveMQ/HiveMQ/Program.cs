using HiveMQtt.Client;
using HiveMQtt.Client.Options;
using HiveMQtt.MQTT5.ReasonCodes;
using HiveMQtt.MQTT5.Types;
using System.Text.Json;

var options = new HiveMQClientOptions
{
    Host = "1481343df7cd494380334fb5dc9a2753.s1.eu.hivemq.cloud",
    Port = 8883,
    UseTLS = true,
    UserName = "cbc1000",
    Password = "Cbc123698745",
};

var client = new HiveMQClient(options);

Console.WriteLine($"Connecting  to {options.Host} on port {options.Port}....");

HiveMQtt.Client.Results.ConnectResult connectResult;

//Connect

try
{
    connectResult = await client.ConnectAsync().ConfigureAwait(false);
    if (connectResult.ReasonCode == ConnAckReasonCode.Success)
    {
        Console.WriteLine($"Connected succesfully to: {connectResult}");
    }
    else
    {
        Console.WriteLine($"Connected Failed to: {connectResult}");
        Environment.Exit(-1);
    }
}
catch (System.Net.Sockets.SocketException e)
{
    Console.WriteLine($"Error connecting to mqtt: {e.Message}");
    Environment.Exit(-1);
}
catch (Exception e)
{
    Console.WriteLine($"Error connecting to mqtt: {e.Message}");
    Environment.Exit(-1);
}

client.OnMessageReceived += (sender, args) =>
{
    string recived_message = args.PublishMessage.PayloadAsString;
    Console.WriteLine(recived_message);
};





while (true)
{
    await client.SubscribeAsync("movement").ConfigureAwait(false);
}