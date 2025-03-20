#!/bin/bash

{ 
  echo -ne "POST /upload HTTP/1.1\r\n"
  echo -ne "Host: 127.0.0.1:8084\r\n"
  echo -ne "Transfer-Encoding: chunked\r\n"
  echo -ne "Content-Type: text/plain\r\n"
  echo -ne "\r\n" # Разделитель между заголовками и телом

  echo -ne "4\r\nda" # Первый чанк
  sleep 2

  echo -ne "ta\r\n5\r\nmored" # Второй чанк
  sleep 2

  echo -ne "\r\n3\r\nfoo\r\n0\r\n\r\n" # Третий чанк + конец запроса
} | nc 127.0.0.1 808