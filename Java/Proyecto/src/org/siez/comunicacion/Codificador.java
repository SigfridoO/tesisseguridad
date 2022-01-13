package org.siez.comunicacion;

public class Codificador {

    public static final String ACTIVAR_ALARMA="%1A2";
    public static final String SI="%+";

    public Codificador(){ //constructor
        System.out.println("Codificador");
    }
    public String codificar(String mensaje){
    return mensaje;
    }
    public String decodificador(String mensaje){
        String respuesta="";

        respuesta=SI;

        return respuesta;
    }
}
