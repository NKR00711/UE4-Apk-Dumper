package ashope.tech.generator;

import static ashope.tech.generator.MainActivity.CheckOverlayPermission;

import android.app.Activity;
import android.os.Bundle;

public class DeleteActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        CheckOverlayPermission(this);
    }
}