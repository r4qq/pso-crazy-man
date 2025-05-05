pub struct Point 
{
    positon: Vec<f64>,
    velocityVector: Vec<f64>,
    personalBest: Vec<f64>,
    grade: f64,
    evaluated: bool,
}

impl Point 
{
    pub fn new(startPos: Vec<f64>, startVelocity: Vec<f64>) -> Self 
    {
        Point {
            positon: startPos.clone(),
            velocityVector: startVelocity,
            personalBest: startPos.clone(),
            grade: f64::MAX,
            evaluated: false,
        }
    }    

    pub fn updatePosition(&mut self) 
    {
        
    }
}