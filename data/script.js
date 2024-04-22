
let config = {};

async function loadConfig() {
    try {
        const response = await fetch('/config');
        if (response.ok) {
            config = await response.json();
            updateUI(); // Update UI elements with new configuration
        } else {
            throw new Error('Failed to load configuration');
        }
    } catch (error) {
        console.error('Error loading configuration:', error);
        alert('Error loading configuration');
    }
}

function updateUI() {
    document.getElementById('wifiSSID').value = config.client.ssid || '';
    document.getElementById('wifiPassword').value = config.client.password || '';
}

window.onload = function () {
    loadConfig();
};

async function saveConfig() {
    try {
        const response = await fetch('/config', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(config), // Use the global config object directly
        });

        if (response.ok) {
            alert('Configuration saved successfully!');
        } else {
            throw new Error('Failed to save configuration');
        }
    } catch (error) {
        console.error('Error saving configuration:', error);
        alert('Error saving configuration');
    }
}

var settingSelect = document.getElementById('settingSelect');
settingSelect.addEventListener('change', function () {
    var setting = settingSelect.value;

    var wifiSettings = document.getElementById('wifi-settings');
    wifiSettings.style.display = 'none';

    var aboutSettings = document.getElementById('about-settings');
    aboutSettings.style.display = 'none';

    var saveSettings = document.getElementById('save-settings');
    saveSettings.style.display = 'none';

    switch (setting) {
        case 'wifi':
            wifiSettings.style.display = 'block';
            saveSettings.style.display = 'block';
            break;
        case 'about':
            aboutSettings.style.display = 'block';
            break;
    }
});

document.getElementById('saveButton').addEventListener('click', function () {
    config.client.ssid = document.getElementById('wifiSSID').value;
    config.client.password = document.getElementById('wifiPassword').value;
    saveConfig();
});


