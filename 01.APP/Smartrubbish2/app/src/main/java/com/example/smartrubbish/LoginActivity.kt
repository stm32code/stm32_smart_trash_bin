package com.example.smartrubbish

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import android.view.View
import com.example.smartrubbish.databinding.ActivityLoginBinding
import com.example.smartrubbish.utils.MToast

import com.gyf.immersionbar.BarHide
import com.gyf.immersionbar.ImmersionBar

class LoginActivity : AppCompatActivity(), View.OnClickListener {
    private lateinit var binding: ActivityLoginBinding
    private var xuehao = "1111"
    private var password = "123456"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)
        InitView()
    }

    private fun InitView() {

        binding.btnLogin.setOnClickListener(this)    //刷新
        binding.etPhone.text = Editable.Factory.getInstance().newEditable(xuehao)
        binding.etPassword.text = Editable.Factory.getInstance().newEditable(password)
        //设置共同沉浸式样式
        initBar()

    }

    //实现默认的沉浸式状态栏样式，特殊的Activity可以通过重写该方法改变状态栏样式，如颜色等
    protected open fun initBar() {

        ImmersionBar.with(this).hideBar(BarHide.FLAG_HIDE_STATUS_BAR).init()
    }

    override fun onClick(v: View?) {
        when (v?.id) {
            //登录
            R.id.btn_login -> {
                Login()
            }
        }
    }

    // 连接
    private fun Login() {
        val user = binding.etPhone.text.toString()
        val paw = binding.etPassword.text.toString()
        if (user == xuehao && paw == password) {
            val intent = Intent()
            //前一个（MainActivity.this）是目前页面，后面一个是要跳转的下一个页面
            intent.setClass(this, MainActivity::class.java)
            startActivity(intent)
        } else MToast.mToast(this, "错误用户或密码")
    }
}