using System;
using System.Text.Json;
using HiveMQtt.Client;
using HiveMQtt.Client.Options;
using HiveMQtt.MQTT5.ReasonCodes;
using HiveMQtt.MQTT5.Types;

var options = new HiveMQClientOptions
{
    Host = "c2e79bd864f242dea67ba553e17b7e22.s2.eu.hivemq.cloud",
    Port = 8883,
    UseTLS = true,
    UserName = "Daniel",
    Password = "Daniel16!",
};

var client = new HiveMQClient(options);

Console.WriteLine($"Connecting to {options.Host} on port {options.Port}....");

HiveMQtt.Client.Results.ConnectResult connectResult;

try
{
    connectResult = await client.ConnectAsync().ConfigureAwait(false);
    if (connectResult.ReasonCode == ConnAckReasonCode.Success)
    {
        Console.WriteLine($"Connect Successful: {connectResult}");
    }
    else
    {
        Console.WriteLine($"Connect failed {connectResult}");
        Environment.Exit(-1);
    }
}
catch (System.Net.Sockets.SocketException e)
{
    Console.WriteLine($"Error connecting to the MQTT Broker with the following message: {e.Message}");
    Environment.Exit(-1);
}

// Subscribe to a specific topic
var topic = "test"; // 

var subscribeResult = await client.SubscribeAsync(topic, QualityOfService.ExactlyOnceDelivery).ConfigureAwait(false);
Console.WriteLine($"Subscribed to {topic}: {subscribeResult.Subscriptions[0].SubscribeReasonCode}");

// Message handler
client.OnMessageReceived += (sender, args) =>
{
    var jsonString = args.PublishMessage.PayloadAsString;
    Console.WriteLine($"Received message: {jsonString}");

    try
    {
        var jsonDocument = JsonDocument.Parse(jsonString);

        var root = jsonDocument.RootElement;
        var message_number = root.GetProperty("MessageNumber").GetInt32();

        Console.WriteLine($"Message Received; topic={args.PublishMessage.Topic}, message number={message_number}");
    }
    catch (JsonException ex)
    {
        Console.WriteLine($"Failed to parse JSON: {ex.Message}");
        Console.WriteLine($"Received message: {args.PublishMessage.PayloadAsString}");
    }
};

Console.WriteLine("Listening for messages. Press any key to exit...");
Console.ReadKey();


