var dgram = require('dgram');
var server = dgram.createSocket('udp4');

const mariadb = require('mariadb');
const pool = mariadb.createPool({
    host: 'localhost',
    user: 'admin',
    password: 'noPassword'
});


server.on('message', (message, rinfo) => {

    var data = JSON.parse(message);
    var ip = rinfo.address;
    var port = rinfo.port;

    if (data.type == "Hello") {
        console.log(`Received Hello from ${ip}:${port}`)

        pool.getConnection()
            .then(conn => {

                var newUid = Math.floor(Math.random() * (9999999 - 1000000 + 1) + 1000000);
                conn.query(`INSERT INTO bitlive.user VALUES(?, INET6_ATON(?), ?)`, [newUid, ip, port]);

                var reply = Buffer.from(JSON.stringify(
                    {
                        type: "PeerRegistered",
                        peerId: newUid
                    }));

                server.send(reply, 0, reply.length, port, ip);
                console.log(`Sent PeerRegistered to ${ip}:${port}`);
                conn.end();
            })
            .catch(err => {
                conn.end();
                console.error(`ERROR: ${err}`);
            })
    }
    else if (data.type == "ReHello") {
        var uid = data.peerId;
        console.log(`Received ReHello from ${uid}:${ip}:${port}`)

        pool.getConnection()
            .then(conn => {
                conn.query(`UPDATE bitlive.user 
                                SET public_ip = INET6_ATON(?), 
                                next_port = ? 
                                WHERE userid = ?`,
                    [ip, port, uid]);

                var reply = Buffer.from(JSON.stringify( { type: "ReHelloAcknowledge" } ));

                server.send(reply, 0, reply.length, port, ip);
                console.log(`Sent ReHelloAckowledge to ${uid}:${ip}:${port}`);
                conn.end();
            })
            .catch(err => {
                conn.end();
                console.error(`ERROR: ${err}`);
            })
    }
    else if (data.type == "PeerConnectRequest") {
        var uid = data.peerId;
        var duid = data.destinationPeerId;

        console.log(`Received PeerConnectRequest from ${uid}:${rinfo.address}:${rinfo.port} for ${duid}`);

        pool.getConnection()
            .then(conn => {
                conn.query(`UPDATE bitlive.user 
                                SET public_ip = INET6_ATON(?), 
                                next_port = ? 
                                WHERE userid = ?`,
                    [ip, port, uid]);

                conn.query(`SELECT userid as peerId, INET6_NTOA(public_ip) as publicIp, next_port as nextPort 
                                FROM bitlive.user 
                                WHERE userid = ${duid}`)
                    .then((rows) => {
                        var sendPacket = rows[0];
                        sendPacket.type = "PeerConnectResponse";
                        var reply = JSON.stringify(sendPacket);

                        server.send(reply, 0, reply.length, port, ip);
                        console.log(`Sent PeerConnectResponse to ${uid} for ${duid} as ${reply}`);

                        conn.end();

                        var dreply = Buffer.from(JSON.stringify({
                            type: "PeerConnectInitiate",
                            peerId: uid,
                            password: data.password,
                            publicIp: ip,
                            nextPort: port
                        }));

                        server.send(dreply, 0, dreply.length, sendPacket['nextPort'], sendPacket['publicIp']);
                        console.log(`Sent PeerConnectResponse to ${duid} for ${uid} as ${dreply}`);
                    })
                    .catch(err => {
                        conn.end();
                        console.error(`ERROR: ${err}`);
                    })
            })
            .catch(err => {
                conn.end();
                console.error(`ERROR: ${err}`);
            })
    }
});

const port = 4855;
const host = '0.0.0.0';

server.bind(port, host);