<?php
include 'config.php';

$horaActual = date('Y/m/d H:i');

// Crear la conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}


// Datos recibidos de ESP32
$serialNumber_device = $_POST['serialNumber_device'];
$tokenAuth = $_POST['tokenAuth'];
$waterLevel = $_POST['waterLevel'];
$releState = $_POST['releState'];
$systemMessage = $_POST['systemMessage'];

/*Obtener el token registrado en la BBDD para el numero de serie del dispositivo que solicita el envio de información. */
$hydrosync_devices = $conn->query("SELECT tokenAuth FROM hydrosync_devices WHERE serialNumber_device = $serialNumber_device");
if ($hydrosync_devices) {
    $row = $hydrosync_devices->fetch_assoc();
    $tokenBBDD = $row['tokenAuth'];
} else {
    echo "El numero de serie del dispositivo es invalido, contacte con un administrador.";
}

if ($tokenAuth === $tokenBBDD) {
    // Datos enviados a la BBDD
    $serialNumber_device = $serialNumber_device;
    $waterLevel = $waterLevel;
    $releState = $releState;
    $systemMessage = $systemMessage;

    // Insertar los datos en la tabla
    $sql = "INSERT INTO hydrosync_logs (serialNumber_device, waterLevel, releState, systemMessage, dateLog) 
            VALUES ('$serialNumber_device', '$waterLevel', '$releState', '$systemMessage', '$horaActual')";

    if ($conn->query($sql) === TRUE) {
        echo "$horaActual: Datos enviados satisfactoriamente desde el dispositivo #$serialNumber_device";
    } else {
        echo "$horaActual: Error al enviar los datos." . $conn->error;
    }

    $conn->close();
} else {
    echo "$horaActual: Error de autentificación, los tokens no coinciden.";
    exit();
}
?>