Clear

$n = 1
$sum = 0
$i = 0

while (1)
{
    $sum += 1/($n * $n)
    $n++

    $my_pi = [System.Math]::Sqrt(6 * $sum)

    $difference = [System.Math]::PI - $my_pi

    if($i -eq 0) 
    {
        Write-Host "Found value: pi ~= $my_pi, Difference: $difference"
    }

    $i++

    if ($i -gt 9999)
    {
        $i = 0
    }
}