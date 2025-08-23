<img src="https://github.com/user-attachments/assets/498281ce-5c39-4279-beb1-f0c6d5b63d20" 
       alt="logoLAN_FILESHARE" 
       width="200" />
       <h1>Nearby File Sender</h1>
<p>A Qt/QML app that discovers devices on your LAN via UDP and establishes a TCP session to share files peer-to-peer.</p>

<h2>Overview</h2>


When the app launches, it broadcasts a <code>UDP</code> discovery packet on the local network and listens for discovery packets from peers. 
Discovered devices appear with their IP address. Clicking a device sends a connection request; when the peer accepts, a confirmation 
message is exchanged and a <code>TCP</code> connection is established. Both sides then share an identical UI for sending and receiving files.
</p>

<h2>Features</h2>
<ul>
  <li>Automatic LAN discovery using <strong>UDP</strong> broadcast/multicast.</li>
  <li>Peer confirmation handshake and <strong>TCP</strong> session setup.</li>
  <li>Bidirectional file sharing with progress feedback.</li>
  <li>Clean, minimal UI built with <strong>QML</strong>.</li>
  <li>Works on the same subnet with no external server.</li>
</ul>

<h2>Quick Start</h2>
<pre>
git clone https://github.com/michelmichy7/LAN_fileshare.git
cd nearby-file-sender
cmake -S . -B build -DCMAKE_PREFIX_PATH="&lt;Qt6 install&gt;"
cmake --build build --config Release
./build/NearbyFileSender
</pre>

<h2>Usage</h2>
<ol>
  <li>Launch the app on two (or more) devices connected to the same network.</li>
  <li>Wait for peers to appear in the device list (discovered via UDP).</li>
  <li>Click a device entry to send a connection request.</li>
  <li>On the peer, accept the request when prompted.</li>
  <li>After the TCP session is established, drag &amp; drop or choose files to send.</li>
</ol>

<h2>License</h2>
<p>MIT License. See <code>LICENSE</code> for details.</p>
