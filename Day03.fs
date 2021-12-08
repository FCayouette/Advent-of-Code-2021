open System.IO

let CountIndex i data = 
    Array.fold(fun ones (x:string) -> match x.[i] with |'1'->ones+1 |_->ones) 0 data
   
let GammaEpsilon counts total =
    let g = Array.fold(fun t c -> if c >= total/2 then t*2 + 1 else t*2) 0 counts
    g * Array.fold(fun t c -> if c < total/2 then t*2 + 1 else t*2) 0 counts

let rec Generator i t f (candidates:string[]) =
    if candidates.Length > 1 then
        let count = CountIndex i candidates
        let choice = if count >= candidates.Length-count then t else f
        Generator (i + 1) t f (Array.filter(fun (x:string) -> x.[i] = choice) candidates)
    else Array.fold(fun t c -> if c ='1' then t*2 + 1 else t*2) 0 (candidates.[0].ToCharArray())

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0])
    let counts = Array.map(fun x -> CountIndex x data) [| 0 .. data.[0].Length - 1 |]
    printfn "Part 1: %A" <| GammaEpsilon counts data.Length
    let o2 = Generator 0 '1' '0' data
    let co2 = Generator 0 '0' '1' data
    printfn "Part 2: %A" (o2 * co2)
    0 
