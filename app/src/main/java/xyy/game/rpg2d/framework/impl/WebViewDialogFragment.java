package xyy.game.rpg2d.framework.impl;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import xyy.game.rpg2d.framework.JNILib;


/**
 * A fullscreen DialogFragment containing only a WebView with transparent background
 *
 * Created by ${XYY} on ${2015/11/20}.
 */

public class WebViewDialogFragment extends ImmersiveDialogFragment {

    private static final String ARG_URL = "url";

    private WebView mWebView;

    public static WebViewDialogFragment newInstance(String url) {
        WebViewDialogFragment fragment = new WebViewDialogFragment();
        Bundle args = new Bundle();
        args.putString(ARG_URL, url);
        fragment.setArguments(args);
        return fragment;
    }

    /**
     * Called to instantiate the view. Creates and returns the WebView.
     */
    @SuppressLint("SetJavaScriptEnabled")
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        /*super.getDialog().setOnKeyListener(new DialogInterface.OnKeyListener() {
            @Override
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP) {
                    onBack();
                    return true; // pretend we've processed it
                } else
                    return false; // pass on to be processed as normal
            }
        });*/

        mWebView = new WebView(getContext());
        mWebView.getSettings().setJavaScriptEnabled(true);
        mWebView.setWebViewClient(new WebViewClient() {
            @TargetApi(Build.VERSION_CODES.LOLLIPOP)
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
                if ("game".equals(request.getUrl().getScheme())) {
                    if("close".equals(request.getUrl().getHost())){
                        dismiss();
                    }
                    return true;
                }
                return false;
            }

            @Override
            public void onPageFinished(WebView view, String url) {

                /*使活动全屏，该Flag可能被OS清除，因此需再次设置 */
                int visibility = View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                        View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_FULLSCREEN;
                /*Android 4.4, API 19
                沉浸式全屏*/
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                    visibility |= View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
                }
                getActivity().getWindow().getDecorView().setSystemUiVisibility(visibility);
            }
        });
        // Prevent selecting
        mWebView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                return true;
            }
        });
        mWebView.setLongClickable(false);
        mWebView.setBackgroundColor(Color.TRANSPARENT);
        mWebView.loadUrl(getArguments().getString(ARG_URL));
        return mWebView;
    }

    /**
     * Called when the fragment is visible to the user and actively running. Resumes the WebView.
     */
    @Override
    public void onPause() {
        super.onPause();
        mWebView.onPause();
    }

    /**
     * Called when the fragment is no longer resumed. Pauses the WebView.
     */
    @Override
    public void onResume() {
        mWebView.onResume();
        super.onResume();
    }

    /**
     * Called when the WebView has been detached from the fragment.
     * The WebView is no longer available after this time.
     */
    @Override
    public void onDestroyView() {
        super.onDestroyView();
    }

    /**
     * Called when the fragment is no longer in use. Destroys the internal state of the WebView.
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        mWebView.destroy();
        JNILib.onWebViewClosed();
    }

    /*public WebView getWebView() {
        return mIsWebViewAvailable ? mWebView : null;
    }*/

}
