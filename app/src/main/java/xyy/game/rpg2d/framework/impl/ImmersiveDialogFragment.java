package xyy.game.rpg2d.framework.impl;

import android.support.v4.app.DialogFragment;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;

/**
 * FullScreen DialogFragment
 * Keep DialogFragment in Immersive mode
 * Created by ${XYY} on ${2017/12/24}.
 */

public abstract class ImmersiveDialogFragment extends DialogFragment {

    /*@Override
    public void setupDialog(Dialog dialog, int style) {
        super.setupDialog(dialog, style);
        AppCompatDelegate.setCompatVectorFromResourcesEnabled(true);
        //Log.i("ImmersiveDialogFragment", "setupDialog");
        // Make the dialog non-focusable before showing it
        Window window = dialog.getWindow();
        if (window != null) {
            //For android 4.0
            window.requestFeature(Window.FEATURE_NO_TITLE);
            window.setFlags(
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);
        }
    }*/

    @Override
    public void onStart() {
        super.onStart();
        //Log.i("ImmersiveDialogFragment", "onStart");
        /*
          Make it fullscreen
         */
        Window window = super.getDialog().getWindow();
        if (window != null) {
            window.setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            window.setBackgroundDrawableResource(android.R.color.transparent);
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        /*
          Keep immersive mode
         */
        Window window = super.getDialog().getWindow();
        if (window != null) {
            window.getDecorView().setSystemUiVisibility(
                    getActivity().getWindow().getDecorView().getSystemUiVisibility()
            );

            //Clear the not focusable flag from the window
            window.clearFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);
        }

        //Update the WindowManager with the new attributes (no nicer way I know of to do this)..
        //WindowManager wm = (WindowManager) getActivity().getSystemService(Context.WINDOW_SERVICE);
        //wm.updateViewLayout(getDialog().getWindow().getDecorView(), getDialog().getWindow().getAttributes());
    }
}