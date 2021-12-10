open System.IO

let rec Iterate i fishes =
    if i = 0 then Array.sum fishes else
    let newFishes = Array.append fishes.[1..] [|fishes.[0]|]
    newFishes.[6] <- newFishes.[6] + fishes.[0]
    Iterate (i-1) newFishes

[<EntryPoint>]
let main argv =
    let fishes = Array.create 9 0UL
    File.ReadAllText(argv.[0]).Split(',') |> Array.map System.Int32.Parse |> Array.countBy id |> Array.iter(fun (index, count) -> fishes.[index] <- (uint64)count)
    printfn "Part 1 %A" <| Iterate 80 fishes
    printfn "Part 2 %A" <| Iterate 256 fishes
    0
