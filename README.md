Project WatchDogs

***The code currently written is in a temporary state, so many things will be reconstructed, redesigned, and restructured.

To test the current code, you can run the scripts in the following order:
	1) server
	2) client
 	3) server2
	4) client2

This will run the server which will wait for a connection from a client, then running the client script will connect to the server and send the contents of the demo.xml file. Running server2 will start up with "second server" which will then wait for a listener. (The clients and servers will be combined for the final version of the project) Then running client2 will have the server send the previous received alert to the listener.


There is some node.js code that has been developed as well but we are unsure as to whether we want to use our time to integrate it with our existing demo.