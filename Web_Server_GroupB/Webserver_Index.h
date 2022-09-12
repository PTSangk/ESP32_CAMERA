/**

  HTML + CSS + JAVA Script


  @
  @
*/


static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>

<head>
    <title>ESP32-CAM Robot</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0px auto;
            padding-top: 30px;
            background-color: #dbdadc;
        }

        table {
            margin-left: auto;
            margin-right: auto;
            width: 100%;
            table-layout: fixed;
        }

        td {
            padding: 8 px;
        }

        .button {
            background-color: #2f4468;
            border: none;
            color: white;
            padding: 10px 20px;
            border-radius: 6px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            float: center;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        img {
            width: auto;
            height: auto;
            max-height: 100%;
            max-width: 100%;
            //-ms-transform: rotate(180deg);
            //transform: rotate(180deg);
        }

        /*
        table,
        th,
        td {
            border: 1px solid black;
            border-collapse: collapse;
        }
       */
    </style>
</head>

<body>
    <h1>ESP32-CAM Robot</h1>
    <table>
        <tr>
            <th>VDO stream</th>
            <th>Control</th>
        </tr>
        <td> <img src="" id="photo"></td>
        <td>
            <table>
                <tr>
                    <td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('car&val=1');"
                            ontouchstart="toggleCheckbox('car&val=1');" onmouseup="toggleCheckbox('car&val=5');"
                            ontouchend="toggleCheckbox('car&val=5');">Forward</button></td>
                </tr>
                <tr>
                    <td align="right"><button class="button" onmousedown="toggleCheckbox('car&val=2');"
                            ontouchstart="toggleCheckbox('car&val=2');" onmouseup="toggleCheckbox('car&val=5');"
                            ontouchend="toggleCheckbox('car&val=5');">Left</button>
                    </td>

                    <td align="center"><button class="button" onmousedown="toggleCheckbox('car&val=5');"
                            ontouchstart="toggleCheckbox('car&val=5');">Stop</button>
                    </td>

                    <td align="left"><button class="button" onmousedown="toggleCheckbox('car&val=3');"
                            ontouchstart="toggleCheckbox('car&val=3');" onmouseup="toggleCheckbox('car&val=5');"
                            ontouchend="toggleCheckbox('car&val=5');">Right</button>
                    </td>
                </tr>

                <tr>

                    <td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('car&val=4');"
                            ontouchstart="toggleCheckbox('car&val=4');" onmouseup="toggleCheckbox('car&val=5');"
                            ontouchend="toggleCheckbox('car&val=5');">Backward</button></td>
                <tr>
                    <div id="sliders" class="slider-container">
                        <td colspan="2">Motor Speed:</td>
                        <td><input type="range" id="speed" min="0" max="255" value="200"
                                onchange="toggleCheckbox('speed&val='+this.value)"></td>
                    </div>
                </tr>

                <tr>
                    <td colspan="2">Raw Value: </td>
                    <td><span id="demo"></span></td>
                </tr>

                <tr>
                    <td colspan="2">Duty Cycle: </td>
                    <td><span id="demo1"></span>
                        <span>%</span>
                    </td>
                </tr>

            </table>
        </td>
    </table>
    <script>
        window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
        function toggleCheckbox(x) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?var=" + x, true);
            xhr.send();
        }

        var slider = document.getElementById("speed");
        var output = document.getElementById("demo");
        var output1 = document.getElementById("demo1");
        output.innerHTML = slider.value;
        output1.innerHTML = ((slider.value * 100) / 255).toFixed(1);
        slider.oninput = function () {
            output.innerHTML = this.value;
            output1.innerHTML = ((this.value * 100) / 255).toFixed(1);
        }
    </script>
</body>
</html>
)rawliteral";
