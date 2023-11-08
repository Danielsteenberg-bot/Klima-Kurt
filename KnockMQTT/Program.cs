using KnockModels;
using HiveMQtt.Client;
using HiveMQtt.Client.Options;
using HiveMQtt.MQTT5.ReasonCodes;
using System.Text.Json;
using KnockMQTT;

var options = new HiveMQClientOptions
{
    Host = "c2e79bd864f242dea67ba553e17b7e22.s2.eu.hivemq.cloud",
    Port = 8883,
    UseTLS = true,
    UserName = "Daniel",
    Password = "Daniel16!",
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
        Console.WriteLine($"Connected succsfully to: {connectResult}");
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

    using var context = new MqttDbContext();

    string recived_message = args.PublishMessage.PayloadAsString;
    Console.WriteLine(recived_message);

    Event event_ = JsonSerializer.Deserialize<Event>(recived_message);

    context.Events.Add(event_);
    context.SaveChanges();

};

while (true)
{
    await client.SubscribeAsync("Movement").ConfigureAwait(false);
}