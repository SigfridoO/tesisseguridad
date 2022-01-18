package org.siez.comunicacion;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class Servidor {
    Servidor(){
        System.out.println("Dentro del constructor");

        try {
            DatagramSocket socketUDP = new DatagramSocket(1234);
            byte[] bufer = new byte[1000];

            while (true) {
                // Construimos el DatagramPacket para recibir peticiones
                DatagramPacket peticion =new DatagramPacket(bufer, bufer.length);

                // Leemos una petición del DatagramSocket
                System.out.println("Esperando una conexiones >>");
                socketUDP.receive(peticion);
                String mensaje= new String(peticion.getData());
                System.out.println(mensaje);


                System.out.print("Datagrama recibido del host: " + peticion.getAddress());
                System.out.println(" desde el puerto remoto: " + peticion.getPort());

                String resp="Desde Servidor";
                // Construimos el DatagramPacket para enviar la respuesta
                DatagramPacket respuesta =new DatagramPacket(resp.getBytes(), resp.length(), peticion.getAddress(), peticion.getPort());

                // Enviamos la respuesta, que es un eco
                socketUDP.send(respuesta);
            }

        } catch (SocketException e) {
            System.out.println("Socket: " + e.getMessage());
        } catch (IOException e) {
            System.out.println("IO: " + e.getMessage());
        }
    }
    public void inicializar(){
        System.out.println("Dentro de inicializar");
    }
}
