using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using KnockAPI.Data;
using KnockModels;

namespace KnockAPI.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class CodesController : ControllerBase
    {
        private readonly DataContext _context;

        public CodesController(DataContext context)
        {
            _context = context;
        }

        // GET: api/Codes
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Code>>> GetCodes()
        {
          if (_context.Codes == null)
          {
              return NotFound();
          }
            return await _context.Codes.ToListAsync();
        }

        // GET: api/Codes/5
        [HttpGet("{id}")]
        public async Task<ActionResult<Code>> GetCode(int id)
        {
          if (_context.Codes == null)
          {
              return NotFound();
          }
            var code = await _context.Codes.FindAsync(id);

            if (code == null)
            {
                return NotFound();
            }

            return code;
        }

        // PUT: api/Codes/5
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPut("{id}")]
        public async Task<IActionResult> PutCode(int id, Code code)
        {
            if (id != code.Id)
            {
                return BadRequest();
            }

            _context.Entry(code).State = EntityState.Modified;

            try
            {
                await _context.SaveChangesAsync();
            }
            catch (DbUpdateConcurrencyException)
            {
                if (!CodeExists(id))
                {
                    return NotFound();
                }
                else
                {
                    throw;
                }
            }

            return NoContent();
        }

        // POST: api/Codes
        // To protect from overposting attacks, see https://go.microsoft.com/fwlink/?linkid=2123754
        [HttpPost]
        public async Task<ActionResult<Code>> PostCode(Code code)
        {
          if (_context.Codes == null)
          {
              return Problem("Entity set 'DataContext.Codes'  is null.");
          }
            _context.Codes.Add(code);
            await _context.SaveChangesAsync();

            return CreatedAtAction("GetCode", new { id = code.Id }, code);
        }

        // DELETE: api/Codes/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteCode(int id)
        {
            if (_context.Codes == null)
            {
                return NotFound();
            }
            var code = await _context.Codes.FindAsync(id);
            if (code == null)
            {
                return NotFound();
            }

            _context.Codes.Remove(code);
            await _context.SaveChangesAsync();

            return NoContent();
        }

        private bool CodeExists(int id)
        {
            return (_context.Codes?.Any(e => e.Id == id)).GetValueOrDefault();
        }
    }
}
