open System.IO

let FindPoints (start:string[]) =
    let mutable result = (Set[])
    for y in [0..start.Length-1] do
        String.iteri(fun x c -> if c='#' then result<-result.Add(x,y)) start.[y]
    result

[<EntryPoint>]
let main argv =
    let raw = File.ReadAllLines(argv.[0]) |> Array.filter(fun (s:string) -> s.Length > 0)
    let decode = raw.[0]

    let mutable points = FindPoints raw.[1..raw.Length-1]
    let blink = decode.[0]='#' && decode.[511]='.'

    let litBounds minX minY maxX maxY = 
        let mutable lit = Set[]
        for x in [minX..maxX] do
            lit <- lit.Add(x, minY).Add(x, maxY)
        for y in [minY+1..maxY-1] do
            lit <- lit.Add(minX, y).Add(maxX, y)
        lit

    let rec Iterate iter (minX, minY, maxX, maxY) (lit:Set<int*int>) =
        if iter=0 then lit.Count else
        let infIsLit = blink && iter%2=1
        let mx, my, mX, mY = minX-1, minY-1, maxX+1, maxY+1
        let allLit = if infIsLit then Set.union lit (litBounds mx my mX mY) else lit
        let outVal = if infIsLit then 1 else 0
        let mutable work = Set[]
        let InBounds x y = mx <= x && my <= y && x <= mX && y <= mY
        let ComputeIndex x y = 
            let mutable i = 0
            for b in [y-1..y+1] do
                for a in [x-1..x+1] do
                    if InBounds a b then 
                        if allLit.Contains(a,b) then i<-(2*i)+1 else i<-2*i
                    else i <- (2*i)+outVal
            i

        for x in [mx..mX] do
            for y in [my..mY] do
                let index = ComputeIndex x y
                if decode.[index] ='#' then 
                    work <- work.Add(x,y)

        Iterate (iter-1) (mx,my,mX,mY) work

    let bounds = (0,0,raw.Length-2,raw.[1].Length-1)
    Iterate 2 bounds points |> printfn "Part 1: %i"
    Iterate 50 bounds points |> printfn "Part 2: %i"
    0
