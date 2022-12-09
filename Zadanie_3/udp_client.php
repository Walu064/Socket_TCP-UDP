#!/usr/bin/php

<?php
    $host = "localhost";
    $port = 8080;

    #Utworzenie gniazda:
    $client = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP) or die("Cannot create socket!");
    #Podłączenie gniazda:
    $result = socket_connect($client, $host, $port) or die("Cannot connect!");

    echo "Wprowadz tekst, ktory zostanie wyslany do serwera.\n";
    echo "Aby zakonczyc wprowadzanie wpisz 'exit' i zatwierdz klawiszem enter.\n";

    #Pobieram PID i wysyłam do serwera:
    $pid = "PID:".getmypid();
    echo "ID Procesu klienta: $pid\n";
    socket_sendto($client, $pid, strlen($pid), 0, $host, $port);

    #Wysyłanie wiadomości:
    $msg = "";
    while(true){
        printf("> ");
        $msg = fgets(STDIN);
        echo "";
        #Sprawdzenie, czy użytkownik chce wyjsc:
        if($msg == "exit\n"){
            break;
        }
        socket_sendto($client, $msg, strlen($msg), 0, $host, $port);
    }

    echo "";
    echo "Zamknieto polaczenie. \n";
    ##Zamknięcie gniazda:
    socket_close($client);
?>