// index.js
var mqtt = require('../../utils/mqtt.min.js')
const app = getApp()
var client = null
Page({
  data: {
    tempo: '0',
    hum: '0',
    lx: '0',
    led: true,
    beep: true
  },
  // 事件处理函数

  onLoad() {
    this.connectmqtt()
  },
  connectmqtt: function () {
    var that = this
    const options = {
      connectTimeout: 4000,
      clientId: 'wsy',
      port: 8084,
      username: '123',
      password: '123'
    }
    client = mqtt.connect('wxs://www.3zzzkkk.cn/mqtt', options)
    client.on('connect', (e) => {
      console.log('服务器连接成功')
      client.subscribe('username', {
        qos: 0
      }, function (err) {
        if (!err) {
          console.log('订阅成功')
        }
      })
    })
    //信息监听事件
    client.on('message', function (username, message) {
      let tem = {}
      tem = JSON.parse(message)
      that.setData({
        tempo: tem.temp,
        hum: tem.humi,
        shidu:tem.shidu,
        lx:tem.Light,
        CO2:tem.CO2
        //    lx:tem.lengtn
      })
      console.log(tem)
      console.log('收到' + message.toString())
    })
    client.on('reconnect', (error) => {
      console.log('正在重新连接', error)
    })
    client.on('error', (error) => {
      console.log('连接失败', error)
    })
  },
  onledchange: function (event) {
    this.setData({
      led:event.detail.value
    })
    console.log(event.detail)
    let sw = event.detail.value
    console.log(event.detail.value)
    if (sw) {
      client.publish('abc', '{"LED_SW":1}', function (err) {
        if (!err) {
          console.log('成功发送指令-开')
        }
      })
    } else {
      client.publish('abc', '{"LED_SW":0}', function (err) {
        if (!err) {
          console.log('成功发送指令-关')
        }
      })
    }
  },
  onbeepchange: function (event) {
    this.setData({
      beep:event.detail.value
    })
    console.log(event.detail)
    let sw = event.detail.value
    console.log(event.detail.value)
    if (sw) {
      client.publish('abc', '{"relay":1}', function (err) {
        if (!err) {
          console.log('成功发送指令-开')
        }
      })
    } else {
      client.publish('abc', '{"relay":0}', function (err) {
        if (!err) {
          console.log('成功发送指令-关')
        }
      })
    }
  },
  save:function() {
    var time= new Date().toJSON().substring(0, 10) + ' ' + new Date().toTimeString().substring(0,8);
    app.globalData.historyData.push({
      tempo: this.data.tempo,
      hum: this.data.hum,
      shidu:this.data.shidu,
      lx:this.data.Light,
      CO2:this.data.CO2,
      led: this.data.led,
      beep: this.data.beep,
      time
    })
    console.log(app.globalData.historyData);
  },
  cancle:function(){
    app.globalData.historyData=[]
  }

})