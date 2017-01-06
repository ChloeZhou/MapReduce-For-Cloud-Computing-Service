all:;
	g++ -o serverAzh serverA.cpp -lsocket -lnsl -lresolv
	g++ -o serverBzh serverB.cpp -lsocket -lnsl -lresolv
	g++ -o serverCzh serverC.cpp -lsocket -lnsl -lresolv
	g++ -o awszh aws.cpp -lsocket -lnsl -lresolv
	g++ -o client client.cpp -lsocket -lnsl -lresolv
serverA:;
	./serverAzh
serverB:;
	./serverBzh
serverC:;
	./serverCzh
aws:;
	./awszh
clean:;
	rm serverAzh serverBzh serverCzh awszh client
