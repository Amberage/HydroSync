<?php
include($_SERVER['DOCUMENT_ROOT'] . '/config/config.php');
// Crear conexión
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar la conexión
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Query para eliminar registros
$sql_delete = "TRUNCATE TABLE hydrosync_logs";

// Ejecutar la query de eliminación
if ($conn->query($sql_delete) === TRUE) {
    echo "Registros eliminados con éxito.";
} else {
    echo "Error al eliminar registros: " . $conn->error;
}

// Query para reiniciar el autoincremento
$sql_alter = "ALTER TABLE estadoHydratech AUTO_INCREMENT = 1";

// Ejecutar la query para reiniciar el autoincremento
if ($conn->query($sql_alter) === TRUE) {
    echo "Autoincremento reiniciado con éxito.";
} else {
    echo "Error al reiniciar el autoincremento: " . $conn->error;
}

// Cerrar la conexión
$conn->close();
?>