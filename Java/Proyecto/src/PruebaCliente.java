package org.siez.comunicacion;

import java.net.DatagramPacket;
import java.net.InetAddress;

public class PruebaCliente {
    public static void main(String[] args) {
        org.siez.comunicacion.Cliente cliente=new org.siez.comunicacion.Cliente();
        cliente.enviarMensaje("");

    }
}