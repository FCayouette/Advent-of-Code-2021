open System.IO

let CharToBits x = 
    let ToBit i p = if i &&& p = p then 1y else 0y
    let i = if x>='A' then (int)x - (int)'A' + 10 else (int)x - (int)'0'
    [|ToBit i 8; ToBit i 4; ToBit i 2; ToBit i 1|]

let ToInt64 = Array.fold(fun t b -> 2L*t + (int64)b) 0L

let DoOp op subPackets =
    let values = Array.map(snd) subPackets
    match op with | 0L -> Array.sum values
                  | 1L -> Array.fold(fun t x -> t*x) 1L values
                  | 2L -> Array.min values
                  | 3L -> Array.max values
                  | 5L -> if values.[0] > values.[1] then 1L else 0L
                  | 6L -> if values.[0] < values.[1] then 1L else 0L
                  | _ -> if values.[0] = values.[1] then 1L else 0L

let rec ReadPacket (message:sbyte[]) start =
    let version = ToInt64 message.[start..start+2]
    let id = ToInt64 message.[start+3..start+5]
    if id = 4L then
        let mutable index = start + 6
        let mutable value = 0L
        let mutable go = true
        while go do
            go <- message.[index] = 1y
            value <- value*16L + (ToInt64 message.[index+1..index+4])
            index <- index + 5
        (index, version, value)
    else
    let mutable subPackets = [||]
    let mutable index = 0
    let Process (t, ver, v) = 
        index <- t
        subPackets <- Array.append subPackets [|(ver, v)|]
    if message.[start+6] = 0y then
        let target = (int)(ToInt64 message.[start+7..start+21]) + start + 22
        index <- start + 22
        while index < target do
            ReadPacket message index |> Process
        (target, Array.fold(fun t (v,_)-> t + v) version subPackets, DoOp id subPackets)
    else
        let subs = (int)(ToInt64 message.[start+7..start+17])
        index <- start+18
        for _ in [1..subs] do
            ReadPacket message index |> Process
        (index, Array.fold(fun t (v,_)-> t + v) version subPackets, DoOp id subPackets)

[<EntryPoint>]
let main argv =
    let bitMessage = File.ReadAllText(argv.[0]).ToCharArray() |> Array.map(CharToBits) |> Array.concat
    let (_, part1, part2) = ReadPacket bitMessage 0
    printfn "Part 1: %i\nPart 2: %i" part1 part2
    0
