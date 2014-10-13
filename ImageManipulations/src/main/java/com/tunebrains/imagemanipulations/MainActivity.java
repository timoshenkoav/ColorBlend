package com.tunebrains.imagemanipulations;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.prophonix.imagemanipulations.R;
import com.tunebrains.graphic.Utils;


public class MainActivity extends ActionBarActivity {

    ImageView mSource;
    ImageView mResult;
    Drawable mSourseDrawable;
    Drawable mResultDrawable;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSource = (ImageView) findViewById(R.id.source);
        mResult = (ImageView) findViewById(R.id.result);
        mSourseDrawable = getResources().getDrawable(R.drawable.stub6);
        mSource.setImageDrawable(mSourseDrawable);
        mResultDrawable = getResources().getDrawable(R.drawable.stub6);
        mResult.setImageDrawable(mResultDrawable);
        findViewById(R.id.process).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View pView) {
                new Process().execute();
            }
        });
    }
    public class Process extends AsyncTask<Void,Void,Void>{

        @Override
        protected Void doInBackground(final Void... pVoids) {
            long start = System.currentTimeMillis();
            Bitmap bmp = ((BitmapDrawable)(mSourseDrawable)).getBitmap();
            Bitmap bmpResult = Bitmap.createBitmap(bmp.getWidth(),bmp.getHeight(), Bitmap.Config.ARGB_8888);
            Utils.grayscale(bmp,bmpResult);
            Utils.colorBlendMode(bmpResult, Color.rgb(84, 79, 102),bmpResult);

            mResultDrawable = new BitmapDrawable(getResources(),bmpResult);
            long end = System.currentTimeMillis();
            Log.w("Duration",""+(end-start));
            return null;
        }

        @Override
        protected void onPostExecute(final Void result) {
            super.onPostExecute(result);
            mResult.setImageDrawable(mResultDrawable);
        }
    }



}
