pub struct Point 
{
    position: Vec<f64>,
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
            position: startPos.clone(),
            velocityVector: startVelocity,
            personalBest: startPos.clone(),
            grade: f64::MAX,
            evaluated: false,
        }
    }    

    pub fn updatePosition(&mut self)
    {
        for i in 0..self.position.len()
        {
            self.position[i] += self.velocityVector[i];
        }
    }

    pub fn updateVelocity(&mut self, alpha: f64, beta: f64, epsilon1: f64, epsilon2: f64, globalBest: &Vec<f64>)
    {
        for i in 0..self.velocityVector.len()
        {
            self.velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - self.position[i]) + 
                                       beta * epsilon2 * (self.personalBest[i] - self.position[i]));
        }
    }

    pub fn enforceBounds(&mut self, bounds: (f64, f64))
    {
        for i in 0..self.velocityVector.len()
        {
            self.position[i] = bounds.0.max(bounds.1.min(self.position[i]));
        }
    }
}