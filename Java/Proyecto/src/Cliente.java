package org.siez.comunicacion;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class Cliente {

    private DatagramSocket socket = null;
    private String hostServidor = "192.168.0.4"; //Arduino
    private int puertoServidor = 1234; //Seleccionar Puerto

    Cliente() {
        try {
            socket = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }

        System.out.println("Dentro del cliente");
    }

    public void enviarMensaje(String mensaje) {
        try {
            // byte[] mensaje = args[0].getBytes();
            // InetAddress hostServidor = InetAddress.getByName(args[1]);

            // Construimos un datagrama para enviar el mensaje al servidor
            DatagramPacket peticion = new DatagramPacket(mensaje.getBytes(), mensaje.length(), InetAddress.getByName(hostServidor), puertoServidor);

            // Enviamos el datagrama
            socket.send(peticion);

            // Construimos el DatagramPacket que contendrá la respuesta
            byte[] bufer = new byte[14];
            DatagramPacket respuesta = new DatagramPacket(bufer, bufer.length);
            socket.receive(respuesta);

            // Enviamos la respuesta del servidor a la salida estandar
            System.out.println("Respuesta: " + new String(respuesta.getData()));

            // Cerramos el socket
            socket.close();

        } catch (SocketException e) {
            System.out.println("Socket: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("IO: " + e.getMessage());
        }
    }
}
