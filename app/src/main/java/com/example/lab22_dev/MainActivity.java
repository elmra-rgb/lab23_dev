package com.example.lab22_dev;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Nouvelles méthodes + anciennes méthodes JNI
    public native boolean isUnderInspection();
    public native String getNativeData();
    public native int calculateFactorial(int n);

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Récupération des vues
        TextView securityTextView = findViewById(R.id.security_status);
        TextView messageTextView = findViewById(R.id.native_message);
        TextView factorialTextView = findViewById(R.id.factorial_result);

        // Appel de la nouvelle méthode anti-debug
        boolean isCompromised = isUnderInspection();

        if (isCompromised) {
            // Mode ALERTE - environnement dangereux
            securityTextView.setText("⚠️ ALERTE SECURITE ⚠️\nEnvironnement suspect détecté !");
            securityTextView.setTextColor(Color.RED);
            securityTextView.setBackgroundColor(Color.parseColor("#FFEBEE"));
            securityTextView.setTextSize(16);

            messageTextView.setText("🔒 FONCTIONS NATIVES DESACTIVEES\nProtection anti-debug active");
            messageTextView.setTextColor(Color.RED);
            messageTextView.setBackgroundColor(Color.parseColor("#FFEBEE"));

            factorialTextView.setText("🚫 CALCUL BLOQUE pour raison de sécurité");
            factorialTextView.setTextColor(Color.RED);
        } 
        else {
            // Mode NORMAL - sécurité OK
            securityTextView.setText("✅ SECURITE OK ✅\nAucune anomalie détectée");
            securityTextView.setTextColor(Color.parseColor("#2E7D32"));
            securityTextView.setBackgroundColor(Color.parseColor("#E8F5E9"));
            securityTextView.setTextSize(16);

            // Anciennes fonctionnalités normales
            String nativeMsg = getNativeData();
            messageTextView.setText(nativeMsg);
            messageTextView.setTextColor(Color.parseColor("#1565C0"));
            messageTextView.setBackgroundColor(Color.parseColor("#E3F2FD"));

            int factorialResult = calculateFactorial(10);
            if (factorialResult >= 0) {
                factorialTextView.setText("📊 Calcul factoriel (10!) = " + factorialResult);
                factorialTextView.setTextColor(Color.parseColor("#1565C0"));
            } else {
                factorialTextView.setText("❌ Erreur dans le calcul factoriel");
                factorialTextView.setTextColor(Color.RED);
            }
        }
    }
}
