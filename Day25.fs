open System.IO

let rec Move (map:char[][]) count = 
    let ShiftRight y= 
        let pairs = Array.pairwise map.[y]
        let lastMoves = map.[y].[0] = '.' && map.[y].[map.[y].Length-1] = '>'
        let hasMoved,_ = Array.fold(fun (t,x) (a,b) -> 
            if a = '>' && b = '.' then 
                map.[y].[x] <- '.' 
                map.[y].[x+1] <- '>'
                (true, x+1)
             else (t, x+1)) (false,0) pairs
        if lastMoves then map.[y].[0] <- '>' 
                          map.[y].[map.[y].Length-1] <- '.'
        lastMoves || hasMoved

    let ShiftDown x =
        let pairs = Array.fold(fun p y -> 
            if y=0 then p else Array.append p [|(map.[y-1].[x], map.[y].[x])|] ) [||] [|0..map.Length-1|]
        let lastMoves = map.[0].[x] = '.' && map.[map.Length-1].[x] = 'v'
        let hasMoved,_ = Array.fold(fun (t,y) (a,b)->
            if a = 'v' && b = '.' then 
                map.[y].[x] <- '.'
                map.[y+1].[x] <- 'v'
                (true, y+1)
            else (t, y+1)) (false, 0) pairs
        if lastMoves then map.[0].[x] <- 'v'
                          map.[map.Length-1].[x] <- '.'
        lastMoves || hasMoved

    let right = Array.fold(fun t y -> if ShiftRight y then true else t) false [|0..map.Length-1|]
    let down  = Array.fold(fun t x -> if ShiftDown  x then true else t) false [|0..map.[0].Length-1|]
    if not right && not down then count else Move map (count+1)

[<EntryPoint>]
let main argv =
    let map = File.ReadAllLines(argv.[0]) |> Array.map(fun (s:string) -> s.ToCharArray())
    printfn "Result %d" <| Move map 1
    0
