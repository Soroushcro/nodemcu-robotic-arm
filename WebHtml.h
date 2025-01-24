const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Arm Control Panel</title>
    <style>
        :root {
            --primary-color: #2196F3;
            --secondary-color: #1976D2;
            --background: #0a192f;
            --text-color: #e6f1ff;
            --slider-height: 8px;
        }

        * {
            margin: 0;
            padding: 0;
        }

        body {
            background: var(--background);
            color: var(--text-color);
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 2rem 1rem;
        }

        h1 {
            font-size: 2.5rem;
            font-weight: 700;
            margin-bottom: 3rem;
            text-transform: uppercase;
            letter-spacing: 2px;
            background: linear-gradient(45deg, var(--primary-color), var(--secondary-color));
            padding: 1rem 2rem;
            border-radius: 10px;
            box-shadow: 0 4px 15px rgba(33, 150, 243, 0.3);
        }

        .container {
            max-width: 1200px;
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 2rem;
            padding: 2rem;
            flex: 1;
        }

        .slidecontainer {
            background: rgba(255, 255, 255, 0.05);
            padding: 2rem;
            border-radius: 15px;
            backdrop-filter: blur(10px);
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
            margin-bottom: 1rem;
            height: 110px;
        }

        .tabs {
            display: flex;
            margin-bottom: 2rem;
            gap: 2rem;
        }

        .tab {
            padding: 1rem 2rem;
            background: rgba(255, 255, 255, 0.05);
            border: none;
            color: var(--text-color);
            cursor: pointer;
            border-radius: 5px;
            width: 160px;
        }

        .tab.active {
            background: var(--primary-color);
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
        }

        .xyz-input {
            background: rgba(255, 255, 255, 0.05);
            padding: 2rem;
            border-radius: 15px;
            max-width: 500px;
            margin: 0 auto;
        }

        .xyz-input input {
            width: 100%;
            padding: 0.5rem;
            margin-bottom: 1rem;
            background: rgba(255, 255, 255, 0.1);
            border: none;
            color: var(--text-color);
            border-radius: 5px;
        }

        label {
            display: block;
            margin-bottom: 1.5rem;
            font-size: 1.2rem;
            font-weight: 500;
            color: var(--primary-color);
        }

        input[type="range"] {
            -webkit-appearance: none;
            appearance: none;
            width: 100%;
            height: var(--slider-height);
            border-radius: 5px;
            background: rgba(255, 255, 255, 0.1);
            outline: none;
            margin: 1rem 0;
        }

        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: var(--primary-color);
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 0 10px rgba(33, 150, 243, 0.5);
        }

        input[type="range"]::-webkit-slider-thumb:hover {
            background: var(--secondary-color);
            transform: scale(1.2);
        }

        input[type="range"]::-moz-range-thumb {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: var(--primary-color);
            cursor: pointer;
            border: none;
            transition: all 0.3s ease;
        }

        input[type="range"]::-moz-range-thumb:hover {
            background: var(--secondary-color);
            transform: scale(1.2);
        }

        input[type="number"] {
            width: 80px;
            padding: 0.5rem;
            background: rgba(255, 255, 255, 0.1);
            border: none;
            color: var(--text-color);
            border-radius: 5px;
            text-align: center;
        }

        footer {
            width: 100%;
            padding: 2rem;
            text-align: center;
            background: rgba(255, 255, 255, 0.03);
            backdrop-filter: blur(10px);
            margin-top: auto;
        }

        .footer-content {
            max-width: 800px;
            margin: 0 auto;
        }

        .university {
            font-size: 1.2rem;
            font-weight: 600;
            margin-bottom: 0.5rem;
            color: var(--primary-color);
        }

        .credits {
            font-size: 0.9rem;
            color: rgba(230, 241, 255, 0.8);
            line-height: 1.6;
        }

        .highlight {
            color: var(--primary-color);
            font-weight: 500;
        }

        @media (max-width: 768px) {
            h1 {
                font-size: 1.8rem;
                padding: 0.8rem 1.5rem;
            }

            .container {
                grid-template-columns: 1fr;
                padding: 1rem;
            }

            .slidecontainer {
                padding: 1.5rem;
                width: 220px;
            }

            .university {
                font-size: 1rem;
            }

            .credits {
                font-size: 0.8rem;
            }
        }

        .submit-btn {
            width: 100%;
            padding: 1rem;
            background: var(--primary-color);
            color: var(--text-color);
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 1.1rem;
            transition: background 0.3s ease;
        }

        .submit-btn:hover {
            background: var(--secondary-color);
        }

        .main-title {
            text-align: center;
        }

        .input-group {
            display: flex;
            align-items: center;
            gap: 1rem;
        }

        .button-group {
            display: flex;
            justify-content: space-between;
            gap: 1rem;
        }

        .control-btn {
            flex: 1;
            padding: 1rem;
            background: var(--primary-color);
            color: var(--text-color);
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background 0.3s ease;
        }

        .control-btn:hover {
            background: var(--secondary-color);
        }
    </style>
</head>

<body>
    <h1 class="main-title">Robotic Arm Control Panel</h1>

    <div class="tabs">
        <button class="tab active" onclick="openTab('sliders')">Sliders</button>
        <button class="tab" onclick="openTab('xyz-position')">XYZ Position</button>
    </div>

    <div id="sliders" class="tab-content active">
        <div class="container">
            <div class="leftside">
                <div class="slidecontainer">
                    <label>Base Rotation</label>
                    <input type="range" min="0" max="180" value="90" id="slider1">
                    <div class="input-group">
                        <input type="number" min="0" max="180" value="90" id="numberInput1">
                    </div>
                </div>

                <div class="slidecontainer">
                    <label>Shoulder</label>
                    <input type="range" min="0" max="180" value="130" id="slider2">
                    <div class="input-group">
                        <input type="number" min="0" max="180" value="130" id="numberInput2">
                    </div>
                </div>

                <div class="slidecontainer">
                    <label>Elbow</label>
                    <input type="range" min="30" max="180" value="50" id="slider3">
                    <div class="input-group">
                        <input type="number" min="30" max="180" value="50" id="numberInput3">
                    </div>
                </div>
            </div>

            <div class="rightside">
                <div class="slidecontainer">
                    <label>Wrist</label>
                    <input type="range" min="0" max="180" value="90" id="slider4">
                    <div class="input-group">
                        <input type="number" min="0" max="180" value="90" id="numberInput4">
                    </div>
                </div>

                <div class="slidecontainer">
                    <label>Wrist Rotation</label>
                    <div class="button-group">
                        <button id="wristLeftBtn" class="control-btn" onclick="sendWristRotation('left')">Left</button>
                        <button id="wristRightBtn" class="control-btn"
                            onclick="sendWristRotation('right')">Right</button>
                    </div>
                </div>

                <div class="slidecontainer">
                    <label>Gripper</label>
                    <div class="button-group">
                        <button class="control-btn" onclick="sendGripperCommand('open')">Open</button>
                        <button class="control-btn" onclick="sendGripperCommand('close')">Close</button>
                        <button class="control-btn" onclick="sendGripperCommand('grip')">Grip</button>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div id="xyz-position" class="tab-content">
        <div class="container">
            <div class="xyz-input">
                <h2>Enter Target Position</h2>
                <div class="param-group">
                    <label>X Position (mm)</label>
                    <input type="number" id="x-pos" value="0" step="1">

                    <label>Y Position (mm)</label>
                    <input type="number" id="y-pos" value="0" step="1">

                    <label>Z Position (mm)</label>
                    <input type="number" id="z-pos" value="0" step="1">
                </div>

                <button onclick="calculateAngles()" class="submit-btn">Calculate Joint Angles</button>

                <div class="result-box">
                    <h3>Calculated Angles:</h3>
                    <p>Base: <span id="base-angle">0</span>&deg;</p>
                    <p>Shoulder: <span id="shoulder-angle">0</span>&deg;</p>
                    <p>Elbow: <span id="elbow-angle">0</span>&deg;</p>
                    <p>Wrist: <span id="wrist-angle">0</span>&deg;</p>
                </div>
            </div>
        </div>
    </div>

    <footer>
        <div class="footer-content">
            <div class="university">Jahrom University &copy; 2024</div>
            <div class="credits">
                Created through the collaborative efforts of<br>
                <span class="highlight">Fatemeh Zolghadri</span> &bull;
                <span class="highlight">Soroush Ebrahimi</span> &bull;
                <span class="highlight">Seyed Ali Sajjadi</span>
            </div>
        </div>
    </footer>

    <script>
        document.querySelectorAll('.slidecontainer').forEach(container => {
            const slider = container.querySelector('input[type="range"]');
            const numberInput = container.querySelector('input[type="number"]');

            if (slider && numberInput) {
                slider.addEventListener('input', () => {
                    numberInput.value = slider.value;
                });

                numberInput.addEventListener('input', () => {
                    const value = Math.min(180, Math.max(0, Number(numberInput.value)));
                    numberInput.value = value;
                    slider.value = value;
                });
            }
        });

        function sendServoPosition(servoNumber, position) {
            var xhttp = new XMLHttpRequest();
            console.log(`moving servo ${servoNumber} to postion ${position}`);
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    console.log(this.responseText);
                }
            };
            xhttp.open("GET", `setPOS?servoPOS${servoNumber}=${position}`, true);
            xhttp.send();
        }

        function sendGripperCommand(command) {
            if (command === 'open') {
                sendServoPosition(6, 'open');
            } else if (command === 'close') {
                sendServoPosition(6, 'close');
            } else if (command === 'grip') {
                sendServoPosition(6, 'grip');
            }
        }

        function sendWristRotation(direction) {
            var position = direction === 'left' ? 75 : 100;
            sendServoPosition(5, position);
        }

        function calculateAngles() {
            const x = document.getElementById('x-pos').value;
            const y = document.getElementById('y-pos').value;
            const z = document.getElementById('z-pos').value;

            fetch(`/calculateIK?x=${x}&y=${y}&z=${z}`)
                .then(response => response.json())
                .then(angles => {
                    document.getElementById('base-angle').textContent = angles.delta.toFixed(1);
                    document.getElementById('shoulder-angle').textContent = angles.theta1.toFixed(1);
                    document.getElementById('elbow-angle').textContent = angles.theta2.toFixed(1);
                    document.getElementById('wrist-angle').textContent = angles.theta3.toFixed(1);

                    const resultBox = document.querySelector('.result-box');
                    if (!document.getElementById('confirm-movement')) {
                        const confirmBtn = document.createElement('button');
                        confirmBtn.id = 'confirm-movement';
                        confirmBtn.className = 'submit-btn';
                        confirmBtn.textContent = 'Confirm Movement';
                        confirmBtn.onclick = () => executeMovement(angles);
                        resultBox.appendChild(confirmBtn);
                    }
                })
                .catch(error => alert('Error: ' + error));
        }

        function executeMovement(angles) {
            fetch(`/moveToXYZ?x=${document.getElementById('x-pos').value}&y=${document.getElementById('y-pos').value}&z=${document.getElementById('z-pos').value}`)
                .then(response => {
                    if (!response.ok) throw new Error(response.statusText);
                    alert('Movement completed');
                })
                .catch(error => alert('Movement failed: ' + error));
        }

        function openTab(tabName) {
            var i, tabContent, tabLinks;
            tabContent = document.getElementsByClassName("tab-content");
            for (i = 0; i < tabContent.length; i++) {
                tabContent[i].style.display = "none";
            }
            tabLinks = document.getElementsByClassName("tab");
            for (i = 0; i < tabLinks.length; i++) {
                tabLinks[i].className = tabLinks[i].className.replace(" active", "");
            }
            document.getElementById(tabName).style.display = "block";
            event.currentTarget.className += " active";
        }

        var slider1 = document.getElementById("slider1");
        var slider2 = document.getElementById("slider2");
        var slider3 = document.getElementById("slider3");
        var slider4 = document.getElementById("slider4");

        var numberInput1 = document.getElementById("numberInput1");
        var numberInput2 = document.getElementById("numberInput2");
        var numberInput3 = document.getElementById("numberInput3");
        var numberInput4 = document.getElementById("numberInput4");

        slider1.oninput = function () {
            sendServoPosition(1, this.value);
        }
        slider2.oninput = function () {
            sendServoPosition(2, this.value);
        }
        slider3.oninput = function () {
            sendServoPosition(3, this.value);
        }
        slider4.oninput = function () {
            sendServoPosition(4, this.value);
        }

        numberInput1.oninput = function () {
            sendServoPosition(1, this.value);
        }
        numberInput2.oninput = function () {
            sendServoPosition(2, this.value);
        }
        numberInput3.oninput = function () {
            sendServoPosition(3, this.value);
        }
        numberInput4.oninput = function () {
            sendServoPosition(4, this.value);
        }
    </script>
</body>

</html>
)=====";
