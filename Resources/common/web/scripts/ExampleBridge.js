/**
 * @file ExampleBridge.js
 * @brief An example Tizen NaCl JavaScript bridge.
 */

"use strict";

function ExampleBridge() {
    var self = this;

    self.nickname = "";
    self.sequentialNumberEventIdCounter = 1;
    self.sequentialNumberEventTimeoutId = undefined;
}

ExampleBridge.getInstance = function getInstance() {
    if(CYIUtilities.isInvalid(ExampleBridge.instance)) {
        ExampleBridge.instance = new ExampleBridge();
    }

    return ExampleBridge.instance;
}

ExampleBridge.prototype.init = function init() {
    var self = this;

    self.nickname = "Example Bridge";

    self.startSequentialNumberEventLoop();
};

ExampleBridge.prototype.getNickname = function getNickname() {
    var self = this;

    return self.nickname;
};

ExampleBridge.prototype.setNickname = function setNickname(nickname) {
    var self = this;

    nickname = CYIUtilities.trimString(nickname, "");

    if(CYIUtilities.isEmptyString(nickname)) {
        throw CYIUtilities.createError("Empty or invalid nickname!");
    }

    self.nickname = nickname;
};

ExampleBridge.getLoadedScripts = function getLoadedScripts() {
    var documentScripts = document.getElementsByTagName("script");
    var scriptSources = [];

    for(var i = 0; i < documentScripts.length; i++) {
        scriptSources.push(documentScripts[i].src);
    }

    return scriptSources;
};

ExampleBridge.prototype.getIPAddress = function getIPAddress(options, callback) {
    var self = this;

    if(CYIUtilities.isFunction(options)) {
        callback = options;
        options = null;
    }

    if(!CYIUtilities.isObjectStrict(options)) {
        options = { };
    }

    options.promisify = CYIUtilities.parseBoolean(options.promisify, true);

    if(!CYIUtilities.isFunction(callback)) {
        if(options.promisify) {
            return CYIUtilities.promisify(self.getIPAddressHelper.bind(self))();
        }
    }
    else {
        return self.getIPAddressHelper(callback);
    }
};

ExampleBridge.prototype.getIPAddressHelper = function getIPAddressHelper(callback) {
    var self = this;

    var request = new XMLHttpRequest();

    request.open("GET", "https://api.ipify.org", true);

    request.timeout = 5000;

    request.addEventListener("load", function(event) {
        var ipAddress = request.responseText;

        if(CYIUtilities.isEmptyString(ipAddress)) {
            return callback(CYIUtilities.createError("Invalid IP address response received!"));
        }

        return callback(null, ipAddress);
    });

    request.addEventListener("error", function(event) {
        return callback(CYIUtilities.createError("Connection failed!"));
    });

    request.send();
};

ExampleBridge.prototype.sendEvent = function sendEvent(eventName, data) {
    var self = this;

    return CYIMessaging.sendEvent({
        context: ExampleBridge.name,
        name: eventName,
        data: data
    });
};

ExampleBridge.prototype.startSequentialNumberEventLoop = function startSequentialNumberEventLoop() {
    var self = this;

    if(CYIUtilities.isValid(self.sequentialNumberEventTimeoutId)) {
        return;
    }

    self.sequentialNumberEventTimeoutId = setTimeout(function() {
        self.sendEvent("sequentialNumber", self.sequentialNumberEventIdCounter++);

        self.sequentialNumberEventTimeoutId = undefined;
        self.startSequentialNumberEventLoop();
    }, Math.floor((Math.random() * 9000) + 1000));
};

ExampleBridge.prototype.stopSequentialNumberEventLoop = function stopSequentialNumberEventLoop() {
    var self = this;

    if(CYIUtilities.isValid(self.sequentialNumberEventTimeoutId)) {
        clearTimeout(self.sequentialNumberEventTimeoutId);
        self.sequentialNumberEventTimeoutId = undefined;
    }
};
