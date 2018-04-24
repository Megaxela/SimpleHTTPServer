#pragma once

#include <cstdint>
#include "Network.hpp"

namespace SocketTools
{
    /**
     * @brief Функция, делающая сокет неблокируемым.
     * @param socket Сокет.
     * @return Успешность установки сокета.
     */
    bool makeSocketNonBlocking(socket_t socket);

    /**
     * @brief Кроссплатформенная функция функция для
     * унификации интерфейса вызова метода recv
     * для разных операционных систем.
     * @param socket Сокет.
     * @param buffer Указатель на буффер, в который должны быть
     * записаны полученные данные.
     * @param len Длина ожидаемых данных.
     * @param flags Флаги, передаваемые в recv.
     * @return Количество полученных данных.
     */
    ssize_t Receive(socket_t socket, uint8_t *buffer, int len, int flags);

    /**
     * @brief Кроссплатформенная функция для
     * унификации интерфейса вызова метода send
     * для разных операционных систем.
     * @param socket Сокет.
     * @param buffer Указатель на буффер откуда
     * будут браться данные для отправки.
     * @param len Длина данных в буффере.
     * @param flags Флаги, передаваемые в send.
     * @return Количество отправленных данных.
     */
    ssize_t Send(socket_t socket, const uint8_t* buffer, int len, int flags);

    /**
     * @brief Кроссплатформенная функция для
     * закрытия сокета.
     * @param socket Сокет.
     */
    void Close(socket_t socket);
}