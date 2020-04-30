# Instructions

## Prérequis:

	yum install alsa-lib-devel
	
## Compilation du client
	./compilation_client.sh
	
## Compilation Serveur
Avec CMake
	
## Synchronisation du serveur
Exécuter la tâche *synch* en prenant soin de modifier les valeurs de l'adresse IP du Raspberry Pi
	
## Rouler le serveur en premier
Sur le Raspberry Pi, faire la commande

	./server
	
À l'endroit où se trouve l'exécutable server
	
## Rouler le client 
À l'endroit où se trouve l'exécutable du client

	./client path_to_file server_ip [compression_frequency]
	
Par exemple:

	./client 8Hz/Crab8.wav 192.42.42.42 8000