## Packet format

A packet consists of N parts:

| Name        | Length            |
|-------------|-------------------|
| timestamp   | 64                |
| type        | 8                 | 
| data_length | 32                |
| data        | data_length bytes |

## Timestamp

The timestamp is a little endian uint64_t representing the time in microseconds since the server started.

## Type

Type is a uint8_t representing the type of packet being sent.

| Sender | In-packet Value | Packet type         |
|--------|-----------------|---------------------|
| Client | 0               | Connection Request  |
| Server | 1               | Syncing Request     |
| Client | 2               | Syncing Response    |
| Server | 3               | Corrected Timestamp |
| Server | 4               | Audio Data          |

### Connection Request
When a client wants to connect to the server, it sends a Connection Request packet. The timestamp of this message should be NULL, and the data will be a TODO! formatted string containing TODO!


### Syncing Request 
The server then replies with a Syncing Request packet. The data part will be NULL.

### Syncing Response
To sync with the server the client then sends a Syncing Response. Once again the timestamp will be NULL, but it's important that the client sends this packet as soon as it's received the connection response packet, which is why the data part will be NULL once again.

### Corrected Timestamp
The server measures the time between sending the connection response and syncing response and then sends another message to the client with a timestamp that has been corrected for the network latency.

### Audio Data
TODO!
