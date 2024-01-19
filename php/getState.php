<?php
$servername = "localhost"; //192.185.131.25
$username = "amberage_root";
$password = "Q2om%)?H.sAQV(r(MD";
$dbname = "amberage_kukultech";

$horaActual = date('Y/m/d H:i');

// Crear la conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}


// Datos recibidos de ESP32
$numeroSerie = $_POST['numeroSerie'];
$tokenAuth = $_POST['tokenAuth'];
$valorSensor = $_POST['valorSensor'];
$valorRele = $_POST['valorRele'];
$valorFlujo = $_POST['valorFlujo'];
$valorError = $_POST['valorError'];
$valorSistema = $_POST['estadoSistema'];

/*Obtener el token registrado en la BBDD para el numero de serie del dispositivo que solicita el envio de información. */
$kukultechDevices = $conn->query("SELECT tokenAuth FROM kukultechDevices WHERE numeroSerie = $numeroSerie");
if ($kukultechDevices) {
    $row = $kukultechDevices->fetch_assoc();
    $tokenBBDD = $row['tokenAuth'];
}

if ($tokenAuth === $tokenBBDD) {
    // Datos enviados a la BBDD
    $numeroSerie = $numeroSerie;
    $estadoSensor = $valorSensor;
    $nivelAgua = '';
    $bombaAgua = '';
    $flujoAgua = '';
    $estadoError = '';
    $estadoSistema = $valorSistema;

    // Pasar de números a texto
    // Nivel de agua:
    if ($valorSensor == "00000000") {
        $nivelAgua = "Menos del 10%";
    } else if ($valorSensor == "00000001") {
        $nivelAgua = "10%";
    } else if ($valorSensor == "00000011") {
        $nivelAgua = "20%";
    } else if ($valorSensor == "00000111") {
        $nivelAgua = "35%";
    } else if ($valorSensor == "00001111") {
        $nivelAgua = "50%";
    } else if ($valorSensor == "00011111") {
        $nivelAgua = "60%";
    } else if ($valorSensor == "00111111") {
        $nivelAgua = "70%";
    } else if ($valorSensor == "01111111") {
        $nivelAgua = "80%";
    } else if ($valorSensor == "11111111") {
        $nivelAgua = "90%";
    } else {
        $nivelAgua = "ERROR SENSOR";
    }

    // Bomba de agua: Encendido o apagado
    if ($valorRele == 1) {
        $bombaAgua = 'Encendida';
    } else {
        $bombaAgua = 'Apagada';
    }

    // Flujo de agua: Sin flujo o Con flujo
    if ($valorFlujo == 1) {
        $flujoAgua = 'Detectado';
    } else {
        $flujoAgua = 'No Detectado';
    }

    // Estado de error
    if ($valorError != 0) {
        $estadoError = 'ERROR!!';
    } else {
        $estadoError = 'OK!';
    }

    // Insertar los datos en la tabla "test"
    $sql = "INSERT INTO estadoHydratech (numeroSerie, estadoSensor, nivelAgua, bombaAgua, flujoAgua, estadoError, estadoSistema) 
            VALUES ('$numeroSerie', '$estadoSensor', '$nivelAgua', '$bombaAgua', '$flujoAgua', '$estadoError', '$estadoSistema')";

    if ($conn->query($sql) === TRUE) {
        echo "$horaActual: Datos enviados satisfactoriamente desde el dispositivo #$numeroSerie";
    } else {
        echo "$horaActual: Error al enviar los datos." . $conn->error;
    }

    $conn->close();
} else {
    echo "$horaActual: Error de autentificación, los tokens no coinciden.";
    exit();
}
?>