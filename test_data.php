<?php
$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

// Connect to MySQL database
$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

// Check if POST data is received
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if temperature, humidity, and ldrValue are set
    if (isset($_POST["temperature"]) && isset($_POST["humidity"]) && isset($_POST["ldrValue"])) {
        $t = $_POST["temperature"];
        $h = $_POST["humidity"];
        $ldr = $_POST["ldrValue"];

        // Insert data into database
        $sql = "INSERT INTO dht22 (temperature, humidity, ldr_value) VALUES ('$t', '$h', '$ldr')";

        if (mysqli_query($conn, $sql)) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . mysqli_error($conn);
        }
    } else {
        echo "Temperature, humidity, or LDR value not set";
    }
}

// Retrieve all data from database
$sql = "SELECT * FROM dht22 ORDER BY id DESC"; // Select all records, ordered by id descending
$result = mysqli_query($conn, $sql);

?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Harshini's Environmental Sensor Data</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            margin: 20px;
        }
        .container {
            max-width: 800px;
            margin: auto;
            background-color: #fff;
            padding: 20px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
            border-radius: 5px;
        }
        h1 {
            text-align: center;
            color: #333;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        table th, table td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: center;
        }
        table th {
            background-color: #f2f2f2;
        }
        .footer {
            text-align: center;
            margin-top: 20px;
            color: #666;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Harshini's Environmental Sensor Data</h1>
        
        <?php
        if (mysqli_num_rows($result) > 0) {
            echo "<table>";
            echo "<tr><th>ID</th><th>Temperature (°C)</th><th>Humidity (%)</th><th>LDR Value</th><th>Timestamp</th><th>Date</th></tr>";

            while ($row = mysqli_fetch_assoc($result)) {
                echo "<tr>";
                echo "<td>" . $row['id'] . "</td>";
                echo "<td>" . $row['temperature'] . "</td>";
                echo "<td>" . $row['humidity'] . "</td>";
                echo "<td>" . $row['ldr_value'] . "</td>";
                echo "<td>" . $row['timestamp'] . "</td>";
                
                // Format timestamp to display date
                $timestamp = strtotime($row['timestamp']);
                $date = date('Y-m-d', $timestamp);
                echo "<td>" . $date . "</td>";
                
                echo "</tr>";
            }

            echo "</table>";
        } else {
            echo "<p>No data found</p>";
        }
        ?>

        <div class="footer">
            <p>© <?php echo date('Y'); ?> Harshini's Environmental Monitoring</p>
        </div>
    </div>
</body>
</html>

<?php
mysqli_close($conn);
?>