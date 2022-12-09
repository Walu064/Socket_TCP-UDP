#!/usr/bin/php

<?php
    $host = "localhost";
    $port = 8080;

    #Utworzenie gniazda:
    $client = socket_create(AF_INET, SOCK_STREAM, 0) or die("Cannot create socket!");
    #Podłączenie gniazda:
    $result = socket_connect($client, $host, $port) or die("Cannot connect!");

    echo "Wprowadz tekst, ktory zostanie wyslany do serwera.\n";
    echo "Aby zakonczyc wprowadzanie wpisz 'exit' i zatwierdz klawiszem enter.\n";

    #Pobieram PID i wysyłam do serwera:
    $pid = "PID:".getmypid();
    echo "ID Procesu klienta: $pid\n";
    socket_send($client, $pid, strlen($pid), 0);

    #Wysyłanie wiadomości:
    $msg = "";
    while(true){
        printf("> ");
        $msg = fgets(STDIN);
        echo "";
        socket_send($client, $msg, strlen($msg), 0);
        #Sprawdzenie, czy użytkownik chce wyjsc:
        if($msg == "exit\n"){
            break;
        }
    }

    echo "";
    echo "Zamknieto polaczenie. \n";
    ##Zamknięcie gniazda:
    socket_close($client);
?>