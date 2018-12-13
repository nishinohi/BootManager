window.onload = function () {
    let succesPop = document.querySelector('.successBox');
    succesPop.addEventListener('transitionend', function (evt) {
        let style = getComputedStyle(succesPop);
        if (style.opacity == 1) {
            succesPop.classList.toggle('show');
        }
    });
    let failedPop = document.querySelector('.failedBox');
    failedPop.addEventListener('transitionend', function (evt) {
        let style = getComputedStyle(failedPop);
        if (style.opacity == 1) {
            failedPop.classList.toggle('show');
        }
    });
}

/**
 * POST メソッドの返り値に対して何かしらの処理を実行するテンプレート関数
 * callBack 関数名は POST リクエストの url と一致させること
 */
function postAndDoAfter(postElements, callBack, url) {
    let xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            if (callBack !== null) {
                callBack(this.responseText);
            }
            togglePopup('.successBox');
        } else if (this.readyState == 4 && this.status != 200) {
            console.error('status:' + String(this.status));
            togglePopup('.failedBox');
        }
    };
    xhr.open('POST', '/' + url, true);
    xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    xhr.send(EncodeJsonToHtmlPost(postElements));
}

// 任意のGPIOのモジュールに電源を供給する
function onPowerSupply() {
    let powerPin = { powerPin: document.getElementById('powerPin').value };
    postAndDoAfter(powerPin, null, onPowerSupply.name);
}

function togglePopup(selector) {
    let popupElement = document.querySelector(selector);
    if (popupElement != null && document.querySelector(selector + '.show') == null) {
        popupElement.classList.toggle('show');
    }
}

// json データを POST パラメータに変換
function EncodeJsonToHtmlPost(data) {
    let params = [];
    for (let name in data) {
        if (data[name] == '' || data[name] == null) {
            continue;
        }
        let value = data[name];
        let param = encodeURIComponent(name) + '=' + encodeURIComponent(value);
        params.push(param);
    }
    return params.join('&').replace(/%20/g, '+');
}