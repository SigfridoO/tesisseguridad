package org.siez.comunicacion;

public class PruebaCodificador {
    public static void main(String[] args) {
        Codificador codificador = new Codificador();
        String mensaje = codificador.codificar(Codificador.ACTIVAR_ALARMA);
        System.out.println(mensaje);

        String respuesta= codificador.decodificador("#_%+2&");
        System.out.println(respuesta);
     }
}
//El mensaje debe quedar con la diferente información que se tiene
