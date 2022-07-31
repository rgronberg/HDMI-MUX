function initIndex() {
    document.getElementById("basic-status").hidden = true
    document.getElementById("js-status").hidden = false

    var buttons = document.getElementsByClassName('portpicker')
    for (let i = 0; i < buttons.length; i++) {
        let button = buttons[i]
        button.href = "javascript:void(0)"
    }

    setSelectedPort()
}

function initReset() {
    document.getElementById("reset-settings").href = "javascript:void(0)"
}

function setSelectedPort() {
    fetch("/selected").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data);
        document.getElementById("status-data").innerHTML = data
    }).catch(function() {
        console.log("Could not fetch current selected port");
    });
}

function port1() {
    fetch("/port1").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data)
        setSelectedPort()
    }).catch(function() {
        console.log("Could not select port1")
    });
}

function port2() {
    fetch("/port2").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data)
        setSelectedPort()
    }).catch(function() {
        console.log("Could not select port2")
    });
}

function port3() {
    fetch("/port3").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data)
        setSelectedPort()
    }).catch(function() {
        console.log("Could not select port3")
    });
}

function port4() {
    fetch("/port4").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data)
        setSelectedPort()
    }).catch(function() {
        console.log("Could not select port4")
    });
}

function nextPort() {
    fetch("/next").then(function(response) {
        return response.text();
    }).then(function(data) {
        console.log(data)
        setSelectedPort()
    }).catch(function() {
        console.log("Could not select next port")
    });
}

function reset() {
    var result = confirm("Would you like to reset WiFi credentials?")

    if (result) {
        window.location.replace("/reallyreset")
    }
    else {
        window.location.replace("/")
    }
}
