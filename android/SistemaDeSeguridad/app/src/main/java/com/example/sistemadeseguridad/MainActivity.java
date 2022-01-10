package com.example.sistemadeseguridad;

import androidx.appcompat.app.AppCompatActivity;

import android.app.IntentService;
import android.os.Bundle;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewDebug;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private View BtnTf;
    private View BtnCd;
    private TextView PTxt;
    private int Contador=0;
    private final String NumEme="Numeros de Emergencia";
    private final String EntSal="Entradas Salidas";
    private int contEJ;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        BtnTf=(ImageButton)findViewById(R.id.imageButtonTf);
        BtnTf.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent=new Intent(MainActivity.this, NumEmerg.class);
                intent.putExtra("Numeros de Emergencia",NumEme);
                startActivity(intent);
            }
        });

        BtnCd=(ImageButton)findViewById(R.id.imageButtonCd);
        BtnCd.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                Intent intent=new Intent(MainActivity.this, EntradasSalidas.class);
                intent.putExtra("Entradas Salidas",EntSal);
                startActivity(intent);
            }
        });
        PTxt=(TextView) findViewById(R.id.PruebaTxt);
        Cont2 contadorEj= new Cont2("Contador");
        contadorEj.start();
    }
    class Cont2 extends Thread {
        public Cont2(String str) {
            super(str); //Herencia de la clase
        }
        public void run(){

            for (int i=0;i<100;i++){
                contEJ++;
                //PTxt.setText(Integer.toString(contEJ));
                Log.i("Probando Hilo", String.valueOf(contEJ));
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}