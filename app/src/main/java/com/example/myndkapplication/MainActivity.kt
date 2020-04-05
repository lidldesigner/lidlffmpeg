package com.example.myndkapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.widget.Toast
import android.widget.Toast.LENGTH_SHORT
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {
    private var mPlayer : Player = Player()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mPlayer.setSurfaceView(surfaceView);
        val file = File(Environment.getExternalStorageDirectory().absolutePath + File.separator + "DCIM/Camera/life.mp4")
        mPlayer.setDataSource(file.absolutePath)
        mPlayer.setOnPrepared{
            runOnUiThread{ Toast.makeText(this, "开始播放", LENGTH_SHORT).show() }
            mPlayer.start();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    override fun onResume() {
        super.onResume()
        mPlayer.prepare();
    }
}

