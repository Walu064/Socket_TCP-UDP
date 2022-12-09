#!/usr/bin/php

<?php
    $host = "localhost";
    $port = 8080;

    $client = socket_create(AF_INET, SOCK_STREAM, 0) or die("Cannot create socket!");
    $result = socket_connect($client, $host, $port) or die("Cannot connect!");

    echo "Wprowadz tekst, ktory zostanie wyslany do serwera.\n";
    echo "Aby zakonczyc wprowadzanie wpisz 'exit' i zatwierdz klawiszem enter.\n";  

    $msg = "";
    while(true){
        printf("> ");
        $msg = fgets(STDIN);
        echo "";
        socket_send($client, $msg, strlen($msg), 0);
        if($msg == "exit\n"){
            break;
        }
    }

    echo "";
    echo "Zamknieto polaczenie. \n";
    socket_close($client);
?>